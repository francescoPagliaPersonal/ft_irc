/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mweghofe <mweghofe@student.42vienna.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/09 14:42:26 by mweghofe          #+#    #+#             */
/*   Updated: 2026/08/13 20:13:22 by mweghofe         ###   ########.fr       */
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

// Add a new channel with TITLE.
void Server::_addChannel(const std::string& title)
{
	Channel* newCh = new Channel(title, "");
	_channels[title] = newCh;
	if (DEBUG)
		std::cout << "[Channel] '" << newCh->getTitle() << "' added.\n";
}

// Remove a channel by channel pointer.
void Server::_removeChannel(Channel* channel)
{
	_channels.erase(channel->getTitle());
	delete channel;
}

// Remove a channel by title.
void Server::_removeChannel(const std::string& title)
{
	Channel* channel = _channels[title];
	_channels.erase(title);
	delete channel;
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
	while (!_channels.empty())
		_removeChannel(_channels.begin()->second);
}
