/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server-Clients.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mweghofe <mweghofe@student.42vienna.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/17 16:27:31 by mweghofe          #+#    #+#             */
/*   Updated: 2026/09/13 10:46:21 by mweghofe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_irc.hpp"
#include "Channel.hpp"
#include "Server.hpp"
#include "Response.hpp"
#include <sstream>
#include <cstddef>
#include <deque>

// -------------------------------------------------------------------------- //
// INTERFACE -- CLIENTS
// -------------------------------------------------------------------------- //

// Lookup client by NICK and return its pointer. Returns NULL if nothing found.
Client* Server::findClientByNick(const std::string & nick) const
{
	std::map<int, Client *>::const_iterator it = _clients.begin();
	while (it != _clients.end())
	{

		if (it->second->getNick() == nick)
			return it->second;
		++it;
	}
	return NULL;
}

// Queue STR for sending to CLIENT and enable the EPOLLOUT interest.
void Server::sendMessage(Client* client, const std::string& str) const
{
	if (client->toBeRemoved())
		return ;
	if (!client->isBufferOutFilled() && !client->hasQuit())
		_epoll.mod(client->getFD(), EPOLL_FL_DEFAULT | EPOLLOUT, client);
	client->putReply2Buff(str);
	// check for clients that don't empty their buffer fast enough
	if (client->isBufferFull(BUF_OUT))
	{
		_addToRemove(client);
		if (DEBUG >= debug::DETAILED)
			std::cout << "[Info] Client '" << client->getNick()
					  << "' (FD " << client->getFD() << ") will be disconnected"
					  << " due to a full outgoing buffer.\n";
	}
}

// -------------------------------------------------------------------------- //
// PRIVATE -- CLIENTS
// -------------------------------------------------------------------------- //

// Try to append a client to the _IPrecords table based on the
// defined MAX_CLIENT_ON_IP limit.
bool Server::_appendToIPrecords(Client* client)
{
	std::map<std::string, std::deque<Client *> >::iterator it;
	std::string IPV4 = client->getHost();
	it = _IPrecords.find(IPV4);
	if (it != _IPrecords.end())
	{
		if (it->second.size() >= MAX_CLIENT_ON_IP)
			return false;
		it->second.push_back(client);
		return true;
	}
	_IPrecords[IPV4].push_back(client);
	if (DEBUG)
	{
		std::cout << "\n[HOST " << IPV4 << "]"
			<< " A new host has been recorded.\n" << std::endl;
	}
	return true;
}

// Remove a client from the _IPrecords table.
void Server::_removeFromIPrecords(Client* client)
{
	std::map<std::string, std::deque<Client*> >::iterator ip;
	ip = _IPrecords.find(client->getHost());
	if (ip == _IPrecords.end())
		throw std::runtime_error("looked for a host that was never registered");
	
	std::deque<Client*>::iterator it;
	for (it = ip->second.begin(); it != ip->second.end(); ++it )
	{
		if (*it == client)
		{
			ip->second.erase(it);
			break ;	
		}
	}
	// any client is in iprecords EXCEPT those that are blocked due to MAX IP
	// EVERY client goes through this function via _deleteClient
	// thus, there are acceptable no-shows => the not allowed clients
	if (ip->second.empty())
	{
		_IPrecords.erase(ip);
		if (DEBUG)
		{
			std::cout << "\n[HOST " << client->getHost() << "]"
				<< " Has been removed from the server.\n" << std::endl;
		}
	}
		
}

// Creates new client and registers FD with epoll.
void Server::_registerNewClient(int fd, const struct sockaddr_in& addr)
{
	Client* tmp;
	// new client, add to map, do sth with addr? IPRecord class?
	try
	{
		tmp = new Client(fd, addr);
	}
	catch (const std::exception& e)
	{
		std::cerr << "[Server] Error while creating new client: " << e.what() << std::endl;
		::close(fd);
		return ;
	}
	// register epoll fd
	try
	{
		_epoll.add(fd, EPOLL_FL_DEFAULT, tmp);
	}
	catch (const std::exception& e)
	{
		std::cerr << "[Server] Error while registering new client: " << e.what() << std::endl;
		delete tmp;
		return ;
	}
	_clients[fd] = tmp;
	if (!_appendToIPrecords(tmp))
	{
		sendMessage(tmp, 
				"ERROR: too many connection from IP " + tmp->getHost() + CRLF);
		_addToRemove(tmp);
		_epoll.mod(fd, EPOLL_FL_QUIT, tmp);
		return ;	
	}
	std::cout << "[Info] New connection from " << tmp->getHost()
			<< " accepted at FD " << fd << '\n';
}
// Remove a client and deregister FD.
void Server::_deleteClient(Client* client)
{
	std::cout << "[Info] Connection to " << client->getHost()
			  << " is being closed on FD " << client->getFD() << ".\n";
	std::set<Client*> contacts;
	if (!client->hasQuit())
	{
		removeClientFromAllChannels(client, &contacts);
		std::stringstream reply;
		reply << ":" << client->getID() << " QUIT :Connection closed." << CRLF;
		broadcast(contacts, reply.str());
	}
	try
	{
		_removeFromIPrecords(client);
	}
	catch (const std::exception& e)
	{
		std::cerr << e.what() << std::endl;
	}
	_epoll.del(client->getFD());
	_clients.erase(client->getFD());
	delete client;
}

// Disconnects a client: remove all pending msgs, channels then client itself.
void Server::_prepareClientDisconnect(Client* client)
{
	// TODO finish this (channels, what else?)
	std::cout << "[Warning] Client removal requested for FD " << client->getFD()
		<< ". Verify implementation.\n";
	_removeMsgsFrom(client);
	_epoll.mod(client->getFD(), EPOLL_FL_QUIT, client);

	// HACK only for testing!!
	// _deleteClient(client);
}

// Inserts a CLIENT that is to be removed into the set of clients to be removed.
void Server::_addToRemove(Client* client) const // with the mutable attribute it has to be const
{
	_toRemove.insert(client);
	client->setRemove(true);
}
