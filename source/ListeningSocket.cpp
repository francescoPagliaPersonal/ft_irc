/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ListeningSocket.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mweghofe <mweghofe@student.42vienna.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/09 17:55:20 by mweghofe          #+#    #+#             */
/*   Updated: 2026/08/10 01:02:16 by mweghofe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ListeningSocket.hpp"

#include <sys/socket.h>		// socket, setsockopt, bind, listen, accept
#include <netinet/in.h>		// struct sockaddr_in
#include <fcntl.h>			// fcntl
#include <unistd.h>			// close

#include <cerrno>
#include <cstring>
#include <string>
#include <stdexcept>


ListeningSocket::~ListeningSocket()
{
	if (fd_ >= 0)
		::close(fd_);
}

// -------------------------------------------------------------------------- //
// OPERATION
// -------------------------------------------------------------------------- //

int ListeningSocket::getFD() const
{
	return (fd_);
}

int ListeningSocket::acceptConnection(struct sockaddr_in& ipAddr)
{
	socklen_t len;
	int newFD;
	
	len = sizeof(ipAddr);
	std::memset(&ipAddr, 0, len);
	// 1) accept the incoming connection
	newFD = accept(fd_, reinterpret_cast<struct sockaddr*>(&ipAddr), &len);
	if (newFD < 0)
	{
		// EAGAIN and EWOULDBLOCK signal the queue is drained, this is OKAY
		if (errno == EAGAIN || errno == EWOULDBLOCK)
			return (-1);
		// anything else is not
		throw std::runtime_error(
			std::string("Error on accept(): ") + std::strerror(errno));
	}
	// 2) make the FD non-blocking
	int flags = ::fcntl(newFD, F_GETFL, 0);
	if (flags < 0 || ::fcntl(newFD, F_SETFL, flags | O_NONBLOCK) < 0)
		throw std::runtime_error(
			std::string("Error on fcntl(): ") + std::strerror(errno));
	return (newFD);
}

// -------------------------------------------------------------------------- //
// OCF
// -------------------------------------------------------------------------- //

ListeningSocket::ListeningSocket()
	: fd_(-1)
{}

ListeningSocket::ListeningSocket(const ListeningSocket& other)
	: fd_(-1)
{
	(void) other;
}

ListeningSocket ListeningSocket::operator=(const ListeningSocket& other)
{
	(void) other;
	return (*this);
}
