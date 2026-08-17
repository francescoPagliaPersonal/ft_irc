/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server-Clients.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mweghofe <mweghofe@student.42vienna.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/17 16:27:31 by mweghofe          #+#    #+#             */
/*   Updated: 2026/08/17 17:45:28 by mweghofe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"
#include <sstream>
// #include <arpa/inet.h> // for the commented out block below

// -------------------------------------------------------------------------- //
// INTERFACE -- CLIENTS
// -------------------------------------------------------------------------- //

// Lookup client by NICK and return its pointer. Returns NULL if nothing found.
Client* Server::findClientByNick(const std::string & nick)
{
	std::map<int, Client *>::iterator it = _clients.begin();
	while (it != _clients.end())
	{

		if (it->second->getNick() == nick)
			return it->second;
		++it;
	}
	return NULL;
}

// Send the welcome message once the client finished the registration handshake.
void Server::tryCompleteRegistration(Client& client)
{
	if (client.getRegistrationFlags() != REG_DONE)
		return ;
	std::stringstream ss;
	ss  << ":CoolServ 001 " << client.getNick()
		<< " :Welcome to the IRC "
		<< client.getNick() << "!" << client.getUserName() << CRLF;
		// << "@" << inet_ntoa(client.addr().sin_addr) << "\r\n";
	std::cout << "[FD " << client.getFD() << "] User registration completed.\n";
	sendMessage(client, ss.str());
}

// Queue STR for sending to CLIENT and enable the EPOLLOUT interest.
void Server::sendMessage(Client& client, const std::string& str)
{
	// TODO drop connection if bufOUT grows too much? or do we drop if kernel buffer stays full?
	client.putReply2Buff(str);
	// TODO consider CATCH & disconnect
	_epoll.mod(client.getFD(), DEF_EPOLL_FL | EPOLLOUT, &client);
}

// -------------------------------------------------------------------------- //
// PRIVATE -- CLIENTS
// -------------------------------------------------------------------------- //

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
		_epoll.add(fd, DEF_EPOLL_FL, tmp);
	}
	catch (const std::exception& e)
	{
		delete tmp;
		throw; // TODO currently this is a hard shutdown; wants sth else
	}
	_clients[fd] = tmp;
}

// Remove a client and deregister FD.
void Server::_removeClient(Client* client)
{
	_epoll.del(client->getFD());
	_clients.erase(client->getFD());
	delete client;
}

// Disconnects a client: remove all pending msgs, channels then client itself.
void Server::_disconnectClient(Client* client)
{
	// TODO finish this (channels, what else?)
	std::cout << "[Warning] Client removal requested for FD " << client->getFD()
		<< ". NOT FULLY IMPLEMENTED yet.\n";
	_removeMsgsFromSuspicious(client);
	_removeClient(client);
}
