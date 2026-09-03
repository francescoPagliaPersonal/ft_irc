/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server-Clients.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fpaglia <fpaglia@student.42vienna.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/17 16:27:31 by mweghofe          #+#    #+#             */
/*   Updated: 2026/09/03 16:23:16 by fpaglia          ###   ########.fr       */
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
	if (!client->isBufferOutFilled() && !client->hasQuit())
		_epoll.mod(client->getFD(), EPOLL_FL_DEFAULT | EPOLLOUT, client);
	// TODO drop connection if bufOUT grows too much? or do we drop if kernel buffer stays full?
	client->putReply2Buff(str);
	// TODO consider CATCH & disconnect
}

// -------------------------------------------------------------------------- //
// PRIVATE -- CLIENTS
// -------------------------------------------------------------------------- //

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
	return true;
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
		::close(fd);
		throw; // TODO currently this is a hard shutdown; wants sth else
	}
	// register epoll fd
	try
	{
		_epoll.add(fd, EPOLL_FL_DEFAULT, tmp);
	}
	catch (const std::exception& e)
	{
		delete tmp;
		throw; // TODO currently this is a hard shutdown; wants sth else
	}
	if (!_appendToIPrecords(tmp))
	{
		sendMessage(tmp, 
				"ERROR: too many connection from IP " + tmp->getHost() + CRLF);
		tmp->setQuit(true);
		_epoll.mod(fd, EPOLL_FL_QUIT, tmp);
		return ;	
	}
	
	_clients[fd] = tmp;
	std::cout << "[Info] New connection from " << tmp->getHost()
			<< " accepted at FD " << fd << '\n';
}
// Remove a client and deregister FD.
void Server::_deleteClient(Client* client)
{
	std::set<Client*> contacts;
	if (!client->hasQuit())
	{
		removeClientFromAllChannels(client, &contacts);
		std::stringstream reply;
		reply << ":" << client->getID() << " QUIT :Connection closed." << CRLF;
		broadcast(contacts, reply.str());
	}
	// TODO remove from _connections
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
