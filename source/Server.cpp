/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mweghofe <mweghofe@student.42vienna.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/09 14:42:26 by mweghofe          #+#    #+#             */
/*   Updated: 2026/08/16 13:51:11 by mweghofe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

// -------------------------------------------------------------------------- //
// OPERATION
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
	// TODO implement this
	std::cout << "[Warning] Client removal requested for FD " << client->getFD()
		<< ". NOT IMPLEMENTED yet.\n";
}

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

void Server::sendMessage(Client& client, const std::string& str)
{
	// TODO does someone need to evaluate how much is in the buffer?
	// TODO   or did we say only the kernel output buffer matters?
	client.putReply2Buff(str);
	// TODO consider CATCH & disconnect
	_epoll.mod(client.getFD(), DEF_EPOLL_FL | EPOLLOUT, &client);
}

std::string Server::getPassword() const
{
	return (_pw);
}

// -------------------------------------------------------------------------- //
// OCF
// -------------------------------------------------------------------------- //

Server::Server()
	: _pw("")
	, _listener(-1)
	, _clients()
	, _epoll()
	, _cmdReg()
{}

Server::Server(const Server& other)
	: _pw("")
	, _listener(-1)
	, _clients()
	, _epoll()
	, _cmdReg()
{
	(void) other;
}

Server Server::operator=(const Server& other)
{
	(void) other;
	return (*this);
}

Server::~Server()
{
	while (!_clients.empty())
		_removeClient(_clients.begin()->second);
}
