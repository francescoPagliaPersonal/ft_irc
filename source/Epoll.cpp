/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Epoll.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mweghofe <mweghofe@student.42vienna.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/09 17:58:17 by mweghofe          #+#    #+#             */
/*   Updated: 2026/08/11 12:14:21 by mweghofe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Epoll.hpp"

#include <stdexcept>
#include <cstring>
#include <cerrno>

#include <unistd.h>

// -------------------------------------------------------------------------- //
// CUSTOM CTOR & DTOR
// -------------------------------------------------------------------------- //

Epoll::Epoll()
	: fd_(-1)
{
	fd_ = ::epoll_create(1);
	if (fd_ < 0)
		throw std::runtime_error(
			std::string("Error on epoll_create(): ") + std::strerror(errno));
}

Epoll::~Epoll()
{
	if (fd_ >= 0)
		::close(fd_);
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

// Registers a new FD with epoll(). This is for the ListeningSocket.
void Epoll::add(int fd, eventflags eventFlags) const
{
	add(fd, eventFlags, NULL);
}

// Registers a new FD with epoll(). Can take a client* as DATA.
void Epoll::add(int fd, eventflags eventFlags, void* data) const
{
	struct epoll_event ev;

	// 1) set up
	std::memset(&ev, 0, sizeof(ev));
	ev.events = eventFlags;
	ev.data.fd = fd;
	if (data)
		ev.data.ptr = data;
	// 2) add new FD to epoll watchlist
	if (::epoll_ctl(fd_, EPOLL_CTL_ADD, fd, &ev) < 0)
		throw std::runtime_error(std::string("Error on epoll_ctl(ADD): ")
			+ std::strerror(errno));
}

void Epoll::mod()
{}

void Epoll::del(int fd) const
{
	::epoll_ctl(fd_, EPOLL_CTL_DEL, fd, NULL);
}

int Epoll::wait(struct epoll_event* ev, int maxEvents, int timeoutMS)
{
	int ready = ::epoll_wait(fd_, ev, maxEvents, timeoutMS);
	if (ready == -1)
		throw std::runtime_error(std::string("Error on epoll_wait(): ")
			+ std::strerror(errno));
	return (ready);
}

// -------------------------------------------------------------------------- //
// OCF
// -------------------------------------------------------------------------- //

Epoll::Epoll(const Epoll& other)
	: fd_(-1)
{
	(void) other;
}

Epoll Epoll::operator=(const Epoll& other)
{
	(void) other;
	return (*this);
}
