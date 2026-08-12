/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mweghofe <mweghofe@student.42vienna.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/09 17:52:36 by mweghofe          #+#    #+#             */
/*   Updated: 2026/08/12 14:18:20 by mweghofe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Client.hpp"

#include <cerrno>

#include <unistd.h>

// TODO remove or modify if client registration doesnt need it => ipAddr stuff
#include <netinet/in.h>

namespace
{
// Render unprintable characters in a string differently.
void printEscaped(std::ostream& os, const std::string& s)
{
	static const char* hex = "0123456789abcdef";

	for (std::size_t i = 0; i < s.size(); i++)
	{
		unsigned char c = static_cast<unsigned char>(s[i]);
		switch (c)
		{
			case '\n': os << "\\n\n"; break;
			case '\r': os << "\\r"; break;
			case '\t': os << "\\t"; break;
			case '\0': os << "\\0"; break;
			default:
				if (c < 0x20 || c == 0x7f)
					os << "\\x" << hex[c >> 4] << hex[c & 0x0f];
				else
					os << static_cast<char>(c);
		}
	}
}
} // end of namespace

// -------------------------------------------------------------------------- //
// CUSTOM CTOR & DTOR
// -------------------------------------------------------------------------- //

Client::Client(int fd, const struct sockaddr_in& addr)
	: _fd(fd)
{
	(void) addr;
}

Client::~Client()
{
	if (_fd > -1)
		::close(_fd);
}

// -------------------------------------------------------------------------- //
// OPERATION
// -------------------------------------------------------------------------- //

int Client::_getFD() const
{
	return (_fd);
}

void Client::_debugWriteToBuffer(const std::string& msg)
{
	bufOUT_.append(msg);
}

e_pollret Client::_receiveToBuffer()
{
	errno = 0;
	char buf[BUF_SIZE + 1];
	ssize_t ret = 0;
	ret = recv(_fd, buf, BUF_SIZE, 0);
	if (ret == -1)
	{
		if (errno == EAGAIN || errno == EWOULDBLOCK) // TODO evalute subject violation
			return (RET_PARSEINPUT);
		return (RET_CLOSE);
	}
	else if (ret == 0) // client disconnected
		return (RET_CLOSE);
	buf[ret] = '\0';
	bufIN_.append(buf);
	if (DEBUG)
	{
		std::cout << "[FD " << _fd << "] Buffer received "<< ret << " chars:\n";
		printEscaped(std::cout, bufIN_);
		std::cout << std::endl;
	}
	return (RET_PARSEINPUT);
}

e_pollret Client::_sendFromBuffer()
{
	ssize_t ret = send(_fd, bufOUT_.c_str(), bufOUT_.size(), 0);
	if (ret < 0)
	{
		if (errno == EAGAIN || errno == EWOULDBLOCK) // TODO evalute subject violation
			return (RET_HASOUTPUT);
		return (RET_CLOSE);	
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
	: _fd(-1)
{}

Client::Client(const Client& other)
	: _fd(-1)
{
	(void) other;
}

Client Client::operator=(const Client& other)
{
	(void) other;
	return (*this);
}
