/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mweghofe <mweghofe@student.42vienna.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/09 14:42:26 by mweghofe          #+#    #+#             */
/*   Updated: 2026/08/09 22:20:01 by mweghofe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"
#include <cstring>

// -------------------------------------------------------------------------- //
// CUSTOM CTOR & DTOR
// -------------------------------------------------------------------------- //

Server::Server(int port, std::string pw)
	: isAlive_(true)
	, port_(port)
	, pw_(pw)
	, socket_(port)
	, clients_()
	, epoll_()
	, cmdReg_()
{
	epoll_.add(socket_.getFD(), EPOLLIN);
	// Client class
	// CommandDispatch class
}

Server::~Server()
{}

// -------------------------------------------------------------------------- //
// MAIN LOOP
// -------------------------------------------------------------------------- //

void Server::run()
{
	struct epoll_event ev[MAX_EVENTS]; // TODO do we need to zero that one?

	while (isAlive_)
	{
		int ready = epoll_.wait(ev, MAX_EVENTS, TIMEOUT);
		// epoll() stuff
		for (int i = 0; i < ready; i++)
		{
			const int fd = ev[i].data.fd;
			// 1) servers's own listening port
			if (fd == socket_.getFD())
			{
				handleListenEvent();
				continue ;
			}
			// 2) normal client (also deals with dispatch)
			handleClientEvent();
		}
		// housekeeping (signal, timeout, sth else?)
	}
}

void Server::handleListenEvent()
{
	std::cout << __FUNCTION__ << " was triggered.\n";
}

void Server::handleClientEvent()
{}

// -------------------------------------------------------------------------- //
// OCF
// -------------------------------------------------------------------------- //

Server::Server()
	: isAlive_(false)
	, port_(-1)
	, pw_("")
	, socket_(-1)
	, clients_()
	, epoll_()
	, cmdReg_()
{}

Server::Server(const Server& other)
	: isAlive_(false)
	, port_(-1)
	, pw_("")
	, socket_(-1)
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
