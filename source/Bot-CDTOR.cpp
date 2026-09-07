/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Bot-CDTOR.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mweghofe <mweghofe@student.42vienna.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/02 08:20:34 by mweghofe          #+#    #+#             */
/*   Updated: 2026/09/07 12:23:34 by mweghofe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Bot.hpp"

#include <sys/socket.h>		// socket, setsockopt, bind, listen, accept
#include <netinet/in.h>		// struct sockaddr_in
#include <netdb.h>			// getprotobyname, struct protoent
#include <fcntl.h>			// fcntl
#include <unistd.h>			// close

#include <cerrno>
#include <cstring>
#include <string>
#include <stdexcept>

namespace {

// Helper to create a raw actie socket for the Bot.
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
	return (fd);
}

// Helper to configure the FD of the listening socket.
void configureFD(int fd)
{	
	// 3) configure socketfd as non-blocking
	int flags = ::fcntl(fd, F_GETFL, 0);
	if (flags < 0 || ::fcntl(fd, F_SETFL, flags | O_NONBLOCK) < 0)
		throw std::runtime_error(
			std::string("Error on fcntl(): ") + std::strerror(errno));
}

// Helper to connect the raw listening socket to the server adddress and port.
// srvAdr is already in network byte order (from arg2ip via getaddrinfo),
void connectAddrToFD(int fd, unsigned int srvAdrNetOrder, unsigned short port)
{
	// 4) connect the new socket to any network adress
	struct sockaddr_in ipAddr;
	std::memset(&ipAddr, 0, sizeof (ipAddr));
	ipAddr.sin_family = AF_INET;
	ipAddr.sin_port = htons(port);
	ipAddr.sin_addr.s_addr = srvAdrNetOrder;
	if (::connect(
			fd, reinterpret_cast<struct sockaddr*>(&ipAddr),
			sizeof(ipAddr)) < 0 && errno != EINPROGRESS
		)
		throw std::runtime_error(
			std::string("Error on connect(): ") + std::strerror(errno));
}

}

Bot::Bot(const unsigned int server,
		 const unsigned short port,
		 const std::string& pw)
	: _fd(-1)
	, _server(server)
	, _port(port)
	, _pw(pw)
{
	_connect();
}

void Bot::_connect()
{
	if (_fd >= 0)
	{
		_epoll.del(_fd);
		::close(_fd);
	}
	_fd = createNewSocket();
	configureFD(_fd);
	connectAddrToFD(_fd, _server, _port);
	_epoll.add(_fd, EPOLLOUT);
}

Bot::~Bot()
{
	::close(_fd);
}
