/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mweghofe <mweghofe@student.42vienna.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/09 14:42:26 by mweghofe          #+#    #+#             */
/*   Updated: 2026/08/11 16:15:01 by mweghofe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

// -------------------------------------------------------------------------- //
// OPERATION
// -------------------------------------------------------------------------- //

// Creates new client and registers FD with epoll.
void Server::registerNewClient(int fd, const struct sockaddr_in& addr)
{
	Client* tmp;
	// new client, add to map, do sth with addr? IPRecord class?
	try
	{
		tmp = new Client(fd, addr, nextClient_);
	}
	catch (const std::exception& e)
	{
		::close(fd);
		throw;
	}
	// register epoll fd
	try
	{
		epoll_.add(fd, DEF_EPOLL_FL);
	}
	catch (const std::exception& e)
	{
		delete tmp;
		throw;
	}
	clients_[fd] = tmp;
	nextClient_++;
}

// Remove a client and deregister FD.
void Server::removeClient(Client* client)
{
	epoll_.del(client->getFD());
	clients_.erase(client->getFD());
	delete client;
}

// -------------------------------------------------------------------------- //
// OCF
// -------------------------------------------------------------------------- //

Server::Server()
	: port_(-1)
	, nextClient_(1)
	, pw_("")
	, listener_(-1)
	, clients_()
	, epoll_()
	, cmdReg_()
{}

Server::Server(const Server& other)
	: port_(-1)
	, nextClient_(1)
	, pw_("")
	, listener_(-1)
	, clients_()
	, epoll_()
	, cmdReg_()
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
	while (!clients_.empty())
		removeClient(clients_.begin()->second);
}
