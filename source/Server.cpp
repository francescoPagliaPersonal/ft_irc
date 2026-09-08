/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mweghofe <mweghofe@student.42vienna.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/09 14:42:26 by mweghofe          #+#    #+#             */
/*   Updated: 2026/09/08 11:53:20 by mweghofe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

// -------------------------------------------------------------------------- //
// OPERATION
// -------------------------------------------------------------------------- //

// Return the server's password.
std::string Server::getPassword() const
{
	return (_pw);
}

// Return server start time.
std::time_t Server::getStartTime() const
{
	return (_startTime);
}

// Return server start time as string.
std::string Server::_getStartTimeString() const
{
	struct tm *local = std::localtime(&_startTime);
	char buf[20];
	std::strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", local);
	return (std::string(buf));
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
