/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mweghofe <mweghofe@student.42vienna.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/09 14:42:26 by mweghofe          #+#    #+#             */
/*   Updated: 2026/08/09 16:49:38 by mweghofe         ###   ########.fr       */
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
{
	if (port_ < 1024 || 65535 < port_)  // TODO  perhaps put this in a parser? due to socket_(port) also needs this check
		throw std::out_of_range("Port number must be between 1024 and 65554.");
	if (pw.empty() == true)
		throw std::out_of_range("Password must not be empty.");
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
{}

Server::Server(const Server& other)
	: isAlive_(false)
	, port_(-1)
	, pw_("")
	, socket_(-1)
	, clients_()
{
	(void) other;
}

Server Server::operator=(const Server& other)
{
	(void) other;
	return (*this);
}
