/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mweghofe <mweghofe@student.42vienna.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/09 17:52:36 by mweghofe          #+#    #+#             */
/*   Updated: 2026/09/02 14:44:23 by mweghofe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Client.hpp"

#include <unistd.h>
#include <arpa/inet.h>

// -------------------------------------------------------------------------- //
// CUSTOM CTOR & DTOR
// -------------------------------------------------------------------------- //

// Custom constructor to set up a new client with FD and ADDR.
Client::Client(int fd, const sockaddr_in& addr)
	: _fd(fd)
	, _registrationFlags(0)
	, _capRequested(false)
	, _hasQuit(false)
	, _nick("*")
	, _address(addr)
	, _noBuffChange(0)
{
	char buf[INET_ADDRSTRLEN];
	if (inet_ntop(AF_INET, &_address.sin_addr, buf, sizeof(buf)) == NULL)
		_host = "0.0.0.0";
	else
		_host = buf;
}

// Closes the client's FD if it is still open.
Client::~Client()
{
	if (!_channels.empty())
		_channels.clear();
	if (_fd > -1)
		::close(_fd);
}

// -------------------------------------------------------------------------- //
// OPERATION
// -------------------------------------------------------------------------- //

// -------------------------------------------------------------------------- //
// OCF
// -------------------------------------------------------------------------- //
