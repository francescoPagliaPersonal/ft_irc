/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Bot.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fpaglia <fpaglia@student.42vienna.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/02 08:12:43 by mweghofe          #+#    #+#             */
/*   Updated: 2026/09/10 15:56:31 by fpaglia          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Bot.hpp"
#include "Epoll.hpp"
#include "ft_irc.hpp"

#include <cstring>
#include <cerrno>
#include <csignal>

#include <sys/epoll.h>
#include <sys/socket.h>

// -------------------------------------------------------------------------- //
// ACTIONS
// -------------------------------------------------------------------------- //

void Bot::_registerWith(const std::string& name)
{
	std::string msg;
	
	msg.append("NICK " + name + CRLF);
	msg.append("USER " + name + " 0 * :" + name + CRLF);
	msg.append("PASS " + _pw + CRLF);

	_bufOUT.append(msg);
	_epoll.mod(_fd, EPOLL_FL_DEFAULT | EPOLLOUT);
}

// -------------------------------------------------------------------------- //
// LEGACY TESTING FUNCTIONS
// -------------------------------------------------------------------------- //

void Bot::_sendToServer(const std::string& msg)
{
	errno = 0;
	if (::send(_fd, msg.c_str(), msg.size(), MSG_NOSIGNAL) < 0)
	{
		std::cerr << "[Error] Sending to server: "
				  << errno << ", " << strerror(errno) << std::endl;
		// _keepRunning = false;
		_hasConn = false;
		if (errno == EPIPE)
			_connect();
	}
	// else
	// 	std::cout << "Sent:\n" << msg;
}

void Bot::_joinChannel(const std::string& channel)
{
	_sendToServer("JOIN " + channel + CRLF);
}

void Bot::_spamUser(const std::string& nick)
{
	_sendToServer("PRIVMSG " + nick + " :" + SPAM_MSG_42 + CRLF);
}

void Bot::_spamChannel(const std::string& channel)
{
	_sendToServer("PRIVMSG " + channel + " :" + SPAM_MSG_CH + CRLF);
}
