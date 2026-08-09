/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ListeningSocket.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mweghofe <mweghofe@student.42vienna.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/09 17:55:20 by mweghofe          #+#    #+#             */
/*   Updated: 2026/08/10 00:23:01 by mweghofe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ListeningSocket.hpp"

#include <sys/socket.h>		// socket, setsockopt, bind, listen, accept
#include <netinet/in.h>		// struct sockaddr_in
#include <netdb.h>			// getprotobyname, struct protoent
#include <fcntl.h>			// fcntl
#include <unistd.h>			// close

#include <cerrno>
#include <cstring>
#include <string>
#include <stdexcept>

// -------------------------------------------------------------------------- //
// INIT HELPER
// -------------------------------------------------------------------------- //
namespace {

int createNewSocket()
{
	int fd;
	struct protoent	*pe;

	// 0) fetch protocol by name
	pe = getprotobyname("tcp");
	if (pe == NULL)
		throw std::runtime_error(
			std::string("Error on getprotobyname(): ") + std::strerror(errno));
	// 1) open a new socket
	fd = socket(AF_INET, SOCK_STREAM, pe->p_proto); // TODO use 0 or pe?
	if (fd < 0)
		throw std::runtime_error(
			std::string("Error on socket(): ") + std::strerror(errno));
	// 2) set operations on the socket
	int optval = 1;
	if (::setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval)) < 0)
		throw std::runtime_error(
			std::string("Error on setsockopt(): ") + std::strerror(errno));
	// TODO do we need SO_KEEPALIVE?
	return (fd);
}

void bindAddrToFD(int fd, unsigned short port)
{
	// 3) bind the new socket to any network adress
	struct sockaddr_in ipAddr;
	std::memset(&ipAddr, 0, sizeof (ipAddr));
	ipAddr.sin_family = AF_INET;
	// convert little-endian numbers to big-endian
	ipAddr.sin_port = htons(port);
	ipAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	if (::bind(fd, reinterpret_cast<struct sockaddr*>(&ipAddr), sizeof(ipAddr)) < 0)
		throw std::runtime_error(
			std::string("Error on bind(): ") + std::strerror(errno));
}

void configureFD(int fd)
{
	// 4) set the socket to active listening
	if (::listen(fd, BACKLOG) < 0)
		throw std::runtime_error(
			std::string("Error on listen(): ") + std::strerror(errno));
	
	// 5) configure socketfd as non-blocking
	int flags = ::fcntl(fd, F_GETFL, 0);
	if (flags < 0 || ::fcntl(fd, F_SETFL, flags | O_NONBLOCK) < 0)
		throw std::runtime_error(
			std::string("Error on fcntl(): ") + std::strerror(errno));
}
}

// -------------------------------------------------------------------------- //
// CUSTOM CTOR & DTOR
// -------------------------------------------------------------------------- //

ListeningSocket::ListeningSocket(unsigned short port)
	: fd_(-1)
{
	fd_ = createNewSocket();
	bindAddrToFD(fd_, port);
	configureFD(fd_);
}

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
