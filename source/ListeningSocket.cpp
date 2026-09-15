/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ListeningSocket.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mweghofe <mweghofe@student.42vienna.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/09 17:55:20 by mweghofe          #+#    #+#             */
/*   Updated: 2026/09/13 12:59:39 by mweghofe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ListeningSocket.hpp"

#include <sys/socket.h>		// socket, setsockopt, bind, listen, accept
#include <fcntl.h>			// fcntl
#include <arpa/inet.h>		// inet_ntoa
#include <netinet/tcp.h>	// TCP_KEEPIDLE, TCP_KEEPINTVL, TCP_KEEPCNT
#include <unistd.h>			// close

#include <cerrno>			// errno
#include <cstring>			// memset, strerror
#include <string>			// string
#include <iostream>			// cerr

// -------------------------------------------------------------------------- //
// OPERATION
// -------------------------------------------------------------------------- //

// Return server listening socket's FD.
int ListeningSocket::getFD() const
{
	return (_fd);
}

// Return server listening socket's port.
int ListeningSocket::getPort() const
{
	return (_port);
}

std::string ListeningSocket::getHostName() const
{
	return (_hostName);
}

// Orchestrates the process of accepting a new connection from IPADDR.
int ListeningSocket::acceptConnection(struct sockaddr_in& ipAddr) const
{
	socklen_t len;
	int newFD;
	
	len = sizeof(ipAddr);
	std::memset(&ipAddr, 0, len);
	errno = 0;
	// 1) accept the incoming connection
	newFD = ::accept(_fd, reinterpret_cast<struct sockaddr*>(&ipAddr), &len);
	if (newFD < 0)
		return (ACCEPT_NONE);
	// 2) make the FD non-blocking
	int flags = ::fcntl(newFD, F_GETFL, 0);
	if (flags < 0 || ::fcntl(newFD, F_SETFL, flags | O_NONBLOCK) < 0)
	{
		_errorOnAcceptConnection(ipAddr);
		::close(newFD);
		return (ACCEPT_SETUP_FAIL);
	}
	// 3) enable keep-alive on the FD
	if (!_enableKeepAlive(newFD))
	{
		_errorOnAcceptConnection(ipAddr);
		::close(newFD);
		return (ACCEPT_SETUP_FAIL);
	}
	return (newFD);
}

bool ListeningSocket::_enableKeepAlive(int fd) const
{
	int optval = 1;
	// 1) enable keep-alive probe on the FD
	if (::setsockopt(fd, SOL_SOCKET, SO_KEEPALIVE, &optval, sizeof(optval)) < 0)
		return (false);
	// 2) Tune the kernel timers
	int idle = ALIVE_TIME_IDLE;
	int interval = ALIVE_TIME_INTERVAL;
	int probes = ALIVE_PROBES;
	// 3) set the kernel timers
	if (::setsockopt(fd, SOL_TCP, TCP_KEEPIDLE, &idle, sizeof(idle)) < 0)
		return (false);
	if (::setsockopt(fd, SOL_TCP, TCP_KEEPINTVL, &interval, sizeof(interval)) < 0)
		return (false);
	if (::setsockopt(fd, SOL_TCP, TCP_KEEPCNT, &probes, sizeof(probes)) < 0)
		return (false);
	return (true);
}

void ListeningSocket::_errorOnAcceptConnection(struct sockaddr_in& ipAddr) const
{
	std::cerr << "[Server] Error while accepting connection from "
		<< inet_ntoa(ipAddr.sin_addr) << ":" << ntohs(ipAddr.sin_port)
		<< ": " << std::strerror(errno) << std::endl;
}

// -------------------------------------------------------------------------- //
// OCF
// -------------------------------------------------------------------------- //

ListeningSocket::ListeningSocket()
	: _fd(-1)
	, _port(-1)
{}

ListeningSocket::ListeningSocket(const ListeningSocket& other)
	: _fd(-1)
	, _port(-1)
{
	(void) other;
}

ListeningSocket ListeningSocket::operator=(const ListeningSocket& other)
{
	(void) other;
	return (*this);
}
