/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mweghofe <mweghofe@student.42vienna.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/09 17:52:36 by mweghofe          #+#    #+#             */
/*   Updated: 2026/08/19 10:28:24 by mweghofe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Client.hpp"

#include <unistd.h>

// -------------------------------------------------------------------------- //
// CUSTOM CTOR & DTOR
// -------------------------------------------------------------------------- //

// Custom constructor to set up a new client with FD and ADDR.
Client::Client(int fd, const struct sockaddr_in& addr)
	: _fd(fd)
	, _registrationFlags(0)
	, _capRequested(false)
	, _nick("*")
{
	(void) addr; // TODO do sth with the address
}

// Closes the client's FD if it is still open.
Client::~Client()
{
	if (_fd > -1)
		::close(_fd);
}

// -------------------------------------------------------------------------- //
// OPERATION
// -------------------------------------------------------------------------- //

// -------------------------------------------------------------------------- //
// OCF
// -------------------------------------------------------------------------- //

Client::Client()
	: _fd(-1)
{}

Client::Client(const Client& other)
	: _fd(-1)
{
	(void) other;
}

Client Client::operator=(const Client& other)
{
	(void) other;
	return (*this);
}
