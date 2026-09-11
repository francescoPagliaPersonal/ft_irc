/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Bot-Buffer.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fpaglia <fpaglia@student.42vienna.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/07 17:16:20 by mweghofe          #+#    #+#             */
/*   Updated: 2026/09/11 17:36:57 by fpaglia          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Bot.hpp"
#include "ft_irc.hpp"
#include "Message.hpp"

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

	// Print received message on one line.
	void printMessageOneLine(const Message& msgs, size_t i)
	{
		std::cout << "[Debug] Msg #" << i
			<< " prefix {"<< msgs.prefix 
			<< "} command {"<< msgs.command
			<< "} params {" ;
		for (size_t j = 0; j < msgs.params.size(); j++)
		{
			if (j != 0)
				std::cout << '|';
			std::cout << msgs.params[j];
		}
		std::cout << "} trailing {" << msgs.trailing << "}\n";
		i++;
	}

} // end of namespace

// -------------------------------------------------------------------------- //
// BUFFER INTERACTIONS
// -------------------------------------------------------------------------- //

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

void Bot::_processInputBuffer()
{
	std::vector<std::string> rawStrs;
	std::string::size_type pos = 0;
	// 1) extract raw string (CRLF) from the input buffer
	//    see Client::getRawStrings
	while (pos != std::string::npos)
	{
		pos = _bufIN.find(CRLF, 0);
		if (pos == std::string::npos)
		{
			if (_bufIN.size() > MSG_MAX_LENGTH)
				rawStrs.push_back(_bufIN);
			break;
		}
		rawStrs.push_back(_bufIN.substr(0, pos));
		_bufIN.erase(0, pos + 2 );
	}
	// 2) check all individual messages & append to msg queue
	for (std::size_t i = 0; i < rawStrs.size(); i++)
	{
		Message tmp = irc::string2Message(rawStrs[i], NULL);
		_msgsQueue.push_back(tmp);
		if (DEBUG) 
			printMessageOneLine(tmp, i + 1);
	}
}
