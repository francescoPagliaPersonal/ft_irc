/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Epoll.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mweghofe <mweghofe@student.42vienna.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/09 17:58:17 by mweghofe          #+#    #+#             */
/*   Updated: 2026/09/11 19:50:57 by mweghofe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Epoll.hpp"

#include <stdexcept>
#include <cstring>
#include <cerrno>
#include <iostream>

#include <unistd.h>

// -------------------------------------------------------------------------- //
// CUSTOM CTOR & DTOR
// -------------------------------------------------------------------------- //

// Create a new epoll instance.
Epoll::Epoll()
	: _fd(-1)
{
	_fd = ::epoll_create(1);
	if (_fd < 0)
		throw std::runtime_error(
			std::string("Error on epoll_create(): ") + std::strerror(errno));
}

// Close the epoll instance.
Epoll::~Epoll()
{
	if (_fd >= 0)
		::close(_fd);
}

// -------------------------------------------------------------------------- //
// OPERATION
// -------------------------------------------------------------------------- //

/*
int epoll_ctl(fd, EPOLL_CTL_ADD, target, &ev)

	ev.data.fd		can take the FD of a client (useful when lookup per FD)
	ev.data.ptr		can take any data pointer, eg. a Client instance
	ev.events		which event(s) to listen for, as bit flags
					EPOLLIN | EPOLLOUT | EPOLLERR | EPOLLHUP
					EPOLLET (edge-trigger, violates poll before every send/recv)
*/

// Register new listening socket FD with epoll().
void Epoll::add(int fd, eventflags eventFlags) const
{
	struct epoll_event ev;

	// 1) set up
	std::memset(&ev, 0, sizeof(ev));
	ev.events = eventFlags;
	ev.data.fd = fd;
	// 2) add new FD to epoll watchlist
	if (::epoll_ctl(_fd, EPOLL_CTL_ADD, fd, &ev) < 0)
		throw std::runtime_error(std::string("Error on epoll_ctl(ADD): ")
			+ std::strerror(errno));
}

// Register new client FD with epoll() and store its pointer for later access.
void Epoll::add(int fd, eventflags eventFlags, Client* client) const
{
	struct epoll_event ev;

	// 1) set up
	std::memset(&ev, 0, sizeof(ev));
	ev.events = eventFlags;
	ev.data.ptr = client;
	// 2) add new FD to epoll watchlist
	if (::epoll_ctl(_fd, EPOLL_CTL_ADD, fd, &ev) < 0)
		throw std::runtime_error(std::string("Error on epoll_ctl(ADD): ")
			+ std::strerror(errno));
}

// Change the set of events to watch for a given FD and store client pointer.
void Epoll::mod(int fd, eventflags eventFlags, Client* client) const
{
	struct epoll_event ev;

	// 1) set up
	std::memset(&ev, 0, sizeof(ev));
	ev.events = eventFlags;
	ev.data.ptr = client;
	// 2) modify fd watchlist
	if (::epoll_ctl(_fd, EPOLL_CTL_MOD, fd, &ev) < 0)
		throw std::runtime_error(std::string("Error on epoll_ctl(MODIFY): ")
			+ std::strerror(errno));
}

#ifdef BONUS
// Change the set of events for the Bot on the server connection.
void Epoll::mod(int fd, eventflags eventFlags) const
{
	struct epoll_event ev;

	// 1) set up
	std::memset(&ev, 0, sizeof(ev));
	ev.events = eventFlags;
	ev.data.fd = fd;
	// 2) modify fd watchlist
	if (::epoll_ctl(_fd, EPOLL_CTL_MOD, fd, &ev) < 0)
		throw std::runtime_error(std::string("Error on epoll_ctl(MODIFY): ")
			+ std::strerror(errno));
}
#endif

// Remove a FD from the epoll() watchlist.
void Epoll::del(int fd) const
{
	// relevant errors here are:
	// EBADF - epoll fd (_fd) is invalid or target fd is invalid
	// ENOENT - target fd is not registered in epoll
	// EINVAL - epoll fd (_fd) is not an epoll fd (eg. _fd == fd)
	// EPERM - target fd doesn't support epoll
	// note: throwing on error breaks cleanup if triggered in class dtor
	//	     kernel also cleans up epoll watchlist on close(fd)
	if (::epoll_ctl(_fd, EPOLL_CTL_DEL, fd, NULL) < 0)
	{
		std::cerr << "[Warning] Removing FD " << fd
			<< " from epoll watchlist caused an error" << std::endl;
	};
}

// Wait up to TIMEOUTMS for events and return the number of ready FDs.
int Epoll::wait(struct epoll_event* ev, int maxEvents, int timeoutMS) const
{
	errno = 0;
	int ready = ::epoll_wait(_fd, ev, maxEvents, timeoutMS);
	if (ready == -1)
	{
		if (errno == EINTR)
			return (0);
		throw std::runtime_error(std::string("Error on epoll_wait(): ")
			+ std::strerror(errno));
	}
	return (ready);
}

// -------------------------------------------------------------------------- //
// OCF
// -------------------------------------------------------------------------- //

Epoll::Epoll(const Epoll& other)
	: _fd(-1)
{
	(void) other;
}

Epoll Epoll::operator=(const Epoll& other)
{
	(void) other;
	return (*this);
}
