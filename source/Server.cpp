/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mweghofe <mweghofe@student.42vienna.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/09 14:42:26 by mweghofe          #+#    #+#             */
/*   Updated: 2026/08/09 21:42:41 by mweghofe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

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
	// what needs to be set up
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
	while (isAlive_)
	{
		// epoll() stuff
		// dispatch
		// housekeeping (signal, timeout, sth else?)
		return ;
	}
}

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
