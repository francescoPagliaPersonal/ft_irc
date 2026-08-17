/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mweghofe <mweghofe@student.42vienna.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/09 17:52:36 by mweghofe          #+#    #+#             */
/*   Updated: 2026/08/17 16:57:08 by mweghofe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Client.hpp"

#include <cerrno>
#include <cstring>

#include <unistd.h>

// TODO remove or modify if client registration doesnt need it => ipAddr stuff
#include <netinet/in.h>

// TODO this file will totally need splitting up

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
	, _registrationFlags(0)
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

// Return Client FD.
int Client::getFD() const
{
	return (_fd);
}

// Retrieve data via recv() once and write it to input buffer on success.
e_pollret Client::receiveToBuffer()
{
	errno = 0;
	char buf[BUF_SIZE + 1];
	ssize_t ret = 0;
	ret = recv(_fd, buf, BUF_SIZE, 0);
	if (ret == -1)
	{
		if (DEBUG)
			std::cerr << "[Error] FD " << _fd << " recv(): "
				<< errno << ", " << strerror(errno) << std::endl;
		return (RET_CLOSE);
	}
	else if (ret == 0) // client disconnected
		return (RET_CLOSE);
	buf[ret] = '\0';
	_bufIN.append(buf);
	if (DEBUG == debug::DETAILED)
	{
		std::cout << "[FD " << _fd << "] Received "<< ret
			<< " chars. Input buffer contains:\n";
		printEscaped(std::cout, _bufIN);
		if (_bufIN.size() && _bufIN[_bufIN.size() - 1] != '\n')
			std::cout << std::endl;
	}
	return (RET_PARSEINPUT);
}

// Send data from the output buffer once via send() and remove it on success.
e_pollret Client::sendFromBuffer()
{
	errno = 0;
	ssize_t ret = send(_fd, _bufOUT.c_str(), _bufOUT.size(), 0);
	if (ret < 0)
	{
		if (DEBUG)
			std::cerr << "[Error] FD " << _fd << " send(): "
				<< errno << ", " << strerror(errno) << std::endl;
		return (RET_CLOSE);	
	}
	if (ret == static_cast<ssize_t>(_bufOUT.size()))
	{
		_bufOUT.clear();
		return (RET_EMPTY);
	}
	else
	{
		_bufOUT = _bufOUT.substr(ret);
		return (RET_HASOUTPUT);
	}
}

void	Client::putReply2Buff(const std::string& str)
{
	
	_bufOUT.append(str);
	if (DEBUG)
		std::cout << "[FD " << _fd << "] Appending to output buffer:\n" << str;
}

bool Client::setRegistrationFlags(int flags)
{
	if (!(_registrationFlags & flags))
	{
		_registrationFlags |= flags;
		return true;
	}
	return false;
}

int Client::getRegistrationFlags() const
{
	return (_registrationFlags);
}

std::string Client::getNick() const
{
	return _nick;
}
void Client::setNick(const std::string & str)
{
	_nick = str;
}

std::string Client::getUserName() const
{
	return _userName;
}

void Client::setUserName(const std::string & str)
{
	_userName = str; 
}

std::string Client::getRealName() const
{
	return _realName;
}

void Client::setRealName(const std::string & str)
{
	_realName = str; 
}

void Client::addChannel(Channel* channel)
{
	_channels.push_back(channel);
}

void Client::removeChannel(Channel* channel)
{
	std::vector<Channel*>::iterator it;
	for (it = _channels.begin(); it != _channels.end(); it++)
	{
		if (*it == channel)
		{
			_channels.erase(it);
			break ;
		}
	}
}

bool Client::isChannelMember(Channel* channel) const
{
	std::vector<Channel*>::const_iterator it;
	for (it = _channels.begin(); it != _channels.end(); it++)
	{
		if (*it == channel)
		{
			return (true);
		}
	}
	return (false);
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
