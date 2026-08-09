/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mweghofe <mweghofe@student.42vienna.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/09 14:42:26 by mweghofe          #+#    #+#             */
/*   Updated: 2026/08/10 00:46:38 by mweghofe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

// -------------------------------------------------------------------------- //
// MAIN LOOP
// -------------------------------------------------------------------------- //

void Server::run()
{
	struct epoll_event ev[MAX_EVENTS]; // TODO do we need to zero that one?

	while (isAlive_)
	{
		int ready = epoll_.wait(ev, MAX_EVENTS, TIMEOUT);
		// 1) epoll() stuff
		for (int i = 0; i < ready; i++)
		{
			const int fd = ev[i].data.fd;
			// A) servers's own listening port
			if (fd == listener_.getFD())
			{
				handleListenEvent();
				continue ;
			}
			// B) normal client (also deals with dispatch)
			handleClientEvent();
		}
		// 2) work dispatch queue
		// 3) housekeeping (signal, timeout, sth else?)
	}
}

// -------------------------------------------------------------------------- //
// OCF
// -------------------------------------------------------------------------- //

Server::Server()
	: port_(-1)
	, pw_("")
	, listener_(-1)
	, clients_()
	, epoll_()
	, cmdReg_()
{}

Server::Server(const Server& other)
	: port_(-1)
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
{}
