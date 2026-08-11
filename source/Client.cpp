/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mweghofe <mweghofe@student.42vienna.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/09 17:52:36 by mweghofe          #+#    #+#             */
/*   Updated: 2026/08/11 12:56:07 by mweghofe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Client.hpp"

#include <unistd.h>

// TODO remove or modify if client registration doesnt need it => ipAddr stuff
#include <netinet/in.h>

// -------------------------------------------------------------------------- //
// CUSTOM CTOR & DTOR
// -------------------------------------------------------------------------- //

Client::~Client()
{
	if (fd_ > -1)
		::close(fd_);
}

// -------------------------------------------------------------------------- //
// OPERATION
// -------------------------------------------------------------------------- //

Client* Client::registerNew(int fd, struct sockaddr_in& addr, int nextClientID)
{
	(void) addr;
	(void) nextClientID;
	if (fd > -1) // HACK remove me
		::close(fd);
	// new client, add to map, do sth with addr? IPRecord class?
	// register epoll fd
	// throw on errors
	return (NULL);
}

int Client::getFD() const
{
	return (fd_);
}

e_pollret Client::receiveToBuffer()
{
	return (RET_OK);
}

e_pollret Client::sendFromBuffer()
{
	return (RET_OK);
}

// -------------------------------------------------------------------------- //
// OCF
// -------------------------------------------------------------------------- //

Client::Client()
	: fd_(-1)
{}

Client::Client(const Client& other)
	: fd_(-1)
{
	(void) other;
}

Client Client::operator=(const Client& other)
{
	(void) other;
	return (*this);
}
