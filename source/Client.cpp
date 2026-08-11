/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mweghofe <mweghofe@student.42vienna.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/09 17:52:36 by mweghofe          #+#    #+#             */
/*   Updated: 2026/08/11 14:38:43 by mweghofe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Client.hpp"

#include <cerrno>

#include <unistd.h>

// TODO remove or modify if client registration doesnt need it => ipAddr stuff
#include <netinet/in.h>

// -------------------------------------------------------------------------- //
// CUSTOM CTOR & DTOR
// -------------------------------------------------------------------------- //

Client::Client(int fd, const struct sockaddr_in& addr, int id)
	: fd_(fd)
	, id_(id)
{
	(void) addr;
}

Client::~Client()
{
	if (fd_ > -1)
		::close(fd_);
}

// -------------------------------------------------------------------------- //
// OPERATION
// -------------------------------------------------------------------------- //

int Client::getFD() const
{
	return (fd_);
}

e_pollret Client::receiveToBuffer()
{
	errno = 0;
	char buf[BUF_SIZE + 1];
	ssize_t ret = 0;
	ret = recv(fd_, buf, BUF_SIZE, 0);
	if (ret == -1)
	{
		if (errno == EAGAIN || errno == EWOULDBLOCK)
			return (RET_AGAIN);
		return (RET_ERROR); // TODO perhaps frame this as RET_CLOSE?
	}
	else if (ret == 0) // client disconnected
		return (RET_ERROR); // TODO perhaps frame this as RET_CLOSE?
	buf[ret] = '\0';
	bufIN_.append(buf);
		std::cout 
		<< "Receiving buffer on fd " << fd_ 
		<< " : "<< bufIN_ << std::endl;
	return (RET_OK);
}

e_pollret Client::sendFromBuffer()
{
	ssize_t ret = send(fd_, bufOUT_.c_str(), bufOUT_.size(), 0);
	if (ret < 0)
	{
		if (errno == EAGAIN || errno == EWOULDBLOCK)
			return (RET_HASOUTPUT);
		return (RET_ERROR);	
	}
	if (ret == static_cast<ssize_t>(bufOUT_.size()))
	{
		bufOUT_.clear();
		return (RET_EMPTY);
	}
	else
	{
		bufOUT_ = bufOUT_.substr(ret);
		return (RET_HASOUTPUT);
	}
}

// -------------------------------------------------------------------------- //
// OCF
// -------------------------------------------------------------------------- //

Client::Client()
	: fd_(-1)
	, id_(0)
{}

Client::Client(const Client& other)
	: fd_(-1)
	, id_(0)
{
	(void) other;
}

Client Client::operator=(const Client& other)
{
	(void) other;
	return (*this);
}
