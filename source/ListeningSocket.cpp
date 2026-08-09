/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ListeningSocket.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mweghofe <mweghofe@student.42vienna.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/09 17:55:20 by mweghofe          #+#    #+#             */
/*   Updated: 2026/08/09 21:43:18 by mweghofe         ###   ########.fr       */
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

// -------------------------------------------------------------------------- //
// CUSTOM CTOR & DTOR
// -------------------------------------------------------------------------- //

ListeningSocket::ListeningSocket(unsigned short port)
	: fd_(-1)
{
	(void) port;
	// 1) open a new socket
	fd_ = socket(AF_INET, SOCK_STREAM, 0);
	if (fd_ < 0)
		throw std::runtime_error(std::string("Error on socket(): ") + std::strerror(errno));
	
	// 2) set operations on the socket
	int optval = 1;
	if (::setsockopt(fd_, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval)) < 0)
		throw std::runtime_error(std::string("Error on setsockopt(): ") + std::strerror(errno));
	
	// 3) bind the new socket to any network adress
	struct sockaddr_in ipAddr;
	std::memset(&ipAddr, 0, sizeof (ipAddr));
	ipAddr.sin_family = AF_INET;
	// convert little-endian numbers to big-endian
	ipAddr.sin_port = htons(port);
	ipAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	if (::bind(fd_, reinterpret_cast<struct sockaddr*>(&ipAddr), sizeof(ipAddr)))
		throw std::runtime_error(std::string("Error on bind(): ") + std::strerror(errno));
	
	// 4) set the socket to active listening
	if (::listen(fd_, BACKLOG) < 0)
		throw std::runtime_error(std::string("Error on listen(): ") + std::strerror(errno));
	
	// 5) configure socketfd as non-blocking
	int flags = ::fcntl(fd_, F_GETFL, 0);
	if (flags < 0 || ::fcntl(fd_, F_SETFL, flags | O_NONBLOCK) < 0)
		throw std::runtime_error(std::string("Error on fcntl(): ") + std::strerror(errno));
	// TODO do we need SO_KEEPALIVE?
}

ListeningSocket::~ListeningSocket()
{
	if (fd_ >= 0)
		::close(fd_);
}

// -------------------------------------------------------------------------- //
// OPERATION
// -------------------------------------------------------------------------- //

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
