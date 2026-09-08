/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Bot-Buffer.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mweghofe <mweghofe@student.42vienna.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/07 17:16:20 by mweghofe          #+#    #+#             */
/*   Updated: 2026/09/08 12:59:51 by mweghofe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Bot.hpp"
#include "ft_irc.hpp"

#include <cerrno>
#include <cstring>

#include <sys/socket.h>

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

irc::epollret Bot::_discardInput()
{
	char buffer[4096];
	ssize_t bytes = ::recv(_fd, buffer, sizeof(buffer), 0);

	if (bytes <= 0)
		return (irc::RET_CLOSE);
	return (irc::RET_OK);
}

void Bot::_processInputBuffer()
{
	// TODO needs to build the msg for command execd
	_bufIN.erase();
}

irc::epollret Bot::_receiveToBuffer()
{
	errno = 0;
	char buf[BUF_SIZE + 1];
	::ssize_t ret = 0;
	ret = recv(_fd, buf, BUF_SIZE, 0);
	if (ret <= 0)
	{
		std::cerr << "[Error] Receiving failed: "
			<< errno << ", " << strerror(errno) << std::endl;
		return (irc::RET_CLOSE);
	}
	buf[ret] = '\0';
	_bufIN.append(buf);
	if (DEBUG == debug::DETAILED)
	{
		std::cout << "[Bot] Received "<< ret
			<< " chars. Input buffer contains:\n";
		printEscaped(std::cout, _bufIN);
		if (_bufIN.size() && _bufIN[_bufIN.size() - 1] != '\n')
			std::cout << std::endl;
	}
	return (irc::RET_PARSEINPUT);
}

irc::epollret Bot::_sendFromBuffer()
{
	errno = 0;
	ssize_t ret = send(_fd, _bufOUT.c_str(), _bufOUT.size(), 0);
	if (ret < 0)
	{
		std::cerr << "[Error] Sending failed: "
			<< errno << ", " << strerror(errno) << std::endl;
		return (irc::RET_CLOSE);	
	}
	if (ret == static_cast<ssize_t>(_bufOUT.size()))
	{
		_bufOUT.clear();
		return (irc::RET_EMPTY);
	}
	else
	{
		_bufOUT = _bufOUT.substr(ret);
		return (irc::RET_HASOUTPUT);
	}
}
