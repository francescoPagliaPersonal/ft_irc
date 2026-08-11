/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Epoll.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mweghofe <mweghofe@student.42vienna.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/09 17:58:17 by mweghofe          #+#    #+#             */
/*   Updated: 2026/08/11 17:35:59 by mweghofe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Epoll.hpp"
#include "Client.hpp"

#include <stdexcept>
#include <cstring>
#include <cerrno>

#include <unistd.h>

// -------------------------------------------------------------------------- //
// CUSTOM CTOR & DTOR
// -------------------------------------------------------------------------- //

Epoll::Epoll()
	: _fd(-1)
{
	_fd = ::epoll_create(1);
	if (_fd < 0)
		throw std::runtime_error(
			std::string("Error on epoll_create(): ") + std::strerror(errno));
}

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

// Registers a new FD with epoll(). Can take a client* as DATA.
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

// Changes the set of events to watch for a given FD.
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

// Removes a FD from the epoll() watchlist.
void Epoll::del(int fd) const
{
	::epoll_ctl(_fd, EPOLL_CTL_DEL, fd, NULL);
}

int Epoll::wait(struct epoll_event* ev, int maxEvents, int timeoutMS)
{
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
