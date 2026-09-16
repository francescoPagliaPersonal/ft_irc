/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client-Buffer.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mweghofe <mweghofe@student.42vienna.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/17 17:02:24 by mweghofe          #+#    #+#             */
/*   Updated: 2026/09/16 13:52:10 by mweghofe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_irc.hpp"
#include "Client.hpp"

#include <cerrno>
#include <cstring>

#include <netinet/in.h>

// -------------------------------------------------------------------------- //
// HELPERS
// -------------------------------------------------------------------------- //

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
// BUFFER INTERACTIONS
// -------------------------------------------------------------------------- //

// Retrieve data via recv() once and write it to input buffer on success.
irc::epollret Client::receiveToBuffer()
{
	errno = 0;
	char buf[BUF_SIZE + 1];
	ssize_t ret = 0;
	ret = recv(_fd, buf, BUF_SIZE, 0);
	// epoll loop is designed as such that EPOLLIN is required to recv()
	// thus the usual EAGAIN cannot happen. any other error is a problem.
	if (ret == -1)
	{
		if (DEBUG)
			std::cerr << "[Error] FD " << _fd << " recv(): "
				<< errno << ", " << strerror(errno) << std::endl;
		return (irc::RET_CLOSE);
	}
	else if (ret == 0) // client disconnected
		return (irc::RET_CLOSE);
	buf[ret] = '\0';
	_bufIN.append(buf);
	// _bufIN can never be > BUF_SIZE + 1
	// because processInputBuffer *always* drains an incomplete stream (no CRLF)
	//  that is longer than MSG_MAX_LENGTH
	if (DEBUG == debug::DETAILED)
	{
		std::cout << "[FD " << _fd << "] Received "<< ret
			<< " chars. Input buffer contains:\n";
		printEscaped(std::cout, _bufIN);
		if (_bufIN.size() && _bufIN[_bufIN.size() - 1] != '\n')
			std::cout << std::endl;
	}
	return (irc::RET_PARSEINPUT);
}

// Send data from the output buffer once via send() and remove it on success.
irc::epollret Client::sendFromBuffer()
{
	errno = 0;
	ssize_t ret = send(_fd, _bufOUT.c_str(), _bufOUT.size(), 0);
	// epoll loop is designed as such that EPOLLOUT is required to send()
	// thus the usual EAGAIN cannot happen. any other error is a problem.
	if (ret < 0)
	{
		if (DEBUG)
			std::cerr << "[Error] FD " << _fd << " send(): "
				<< errno << ", " << strerror(errno) << std::endl;
		return (irc::RET_CLOSE);
	}
	if (ret == static_cast<ssize_t>(_bufOUT.size()))
	{
		_bufOUT.clear();
		if (_hasQuit)
			return (irc::RET_CLOSE);
		else
			return (irc::RET_EMPTY);
	}
	else
	{
		_bufOUT = _bufOUT.substr(ret);
		std::cout << '[' << __FUNCTION__ << "] kernel buffer doesn't have enough space.\n";
		return (irc::RET_OK);
	}
}

// Append STR to the client's output buffer for later sending.
void	Client::putReply2Buff(const std::string& str)
{
	
	_bufOUT.append(str);
	if (DEBUG)
		std::cout << "[FD " << _fd << "] Appending to output buffer:\n" << str;
}
