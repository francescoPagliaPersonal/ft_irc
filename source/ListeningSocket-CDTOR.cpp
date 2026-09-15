/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ListeningSocket-CDTOR.cpp                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mweghofe <mweghofe@student.42vienna.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/10 01:00:26 by mweghofe          #+#    #+#             */
/*   Updated: 2026/09/13 09:32:28 by mweghofe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ListeningSocket.hpp"
#include "ft_irc.hpp"

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


// Helper to create a raw listening socket for the server.
int ListeningSocket::_createNewSocket()
{
	int fd;
	struct protoent	*pe;

	// 0) fetch protocol by name
	pe = ::getprotobyname("tcp");
	if (pe == NULL)
		throw std::runtime_error(
			std::string("Error on getprotobyname(): ") + std::strerror(errno));
	// 1) open a new socket
	fd = ::socket(AF_INET, SOCK_STREAM, pe->p_proto); // TODO use 0 or pe?
	if (fd < 0)
		throw std::runtime_error(
			std::string("Error on socket(): ") + std::strerror(errno));
	// 2) set operations on the socket
	int optval = 1;
	if (::setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval)) < 0)
		throw std::runtime_error(
			std::string("Error on setsockopt(): ") + std::strerror(errno));
	return (fd);
}

// Helper to configure the FD of the listening socket.
void ListeningSocket::_configureFD()
{
	// 4) set the socket to active listening
	if (::listen(_fd, BACKLOG) < 0)
		throw std::runtime_error(
			std::string("Error on listen(): ") + std::strerror(errno));
	
	// 5) configure socketfd as non-blocking
	int flags = ::fcntl(_fd, F_GETFL, 0);
	if (flags < 0 || ::fcntl(_fd, F_SETFL, flags | O_NONBLOCK) < 0)
		throw std::runtime_error(
			std::string("Error on fcntl(): ") + std::strerror(errno));
}

// Helper to bind the raw listening socket to a network adddress and port.
void ListeningSocket::_bindAddrToFD()
{
	// 3) bind the new socket to any network adress
	std::memset(&_ipAddr, 0, sizeof (_ipAddr));
	_ipAddr.sin_family = AF_INET;
	// convert little-endian numbers to big-endian
	_ipAddr.sin_port = ::htons(_port);
	_ipAddr.sin_addr.s_addr = ::htonl(INADDR_ANY);
	if (::bind(_fd, reinterpret_cast<struct sockaddr*>(&_ipAddr), sizeof(_ipAddr)) < 0)
		throw std::runtime_error(
			std::string("Error on bind(): ") + std::strerror(errno));
}

void ListeningSocket::_setHostName()
{
	char buffer[30];
	if (::gethostname(buffer, 30) == -1)
		_hostName = SERVER_NAME;
	else
		_hostName = std::string(buffer);
}
// -------------------------------------------------------------------------- //
// CUSTOM CTOR
// -------------------------------------------------------------------------- //

// Custom constructor to create the server's listening socket.
ListeningSocket::ListeningSocket(unsigned short port)
	: _fd(-1)
	, _port(port)
{
	_fd = _createNewSocket();
	_bindAddrToFD();
	_configureFD();
	_setHostName();
}

// Close the listening socket's FD.
ListeningSocket::~ListeningSocket()
{
	if (_fd >= 0)
		::close(_fd);
}
