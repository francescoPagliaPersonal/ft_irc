/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Bot.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fpaglia <fpaglia@student.42vienna.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/02 08:12:43 by mweghofe          #+#    #+#             */
/*   Updated: 2026/09/11 16:58:17 by fpaglia          ###   ########.fr       */
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

void Bot::sendMessage(const std::string& msg)
{
	// (void) msg;
	if (_bufOUT.empty())
		_epoll.mod(_fd, EPOLL_FL_DEFAULT | EPOLLOUT, NULL);
	_bufOUT.append(msg);
	if (DEBUG)
		std::cout << "[Bot] Appending to output buffer:\n" << msg;
}

// -------------------------------------------------------------------------- //
// ACTIONS
// -------------------------------------------------------------------------- //

void Bot::_registerWith(const std::string& name)
{
	std::string msg;
	
	msg.append("NICK " + name + CRLF);
	msg.append("USER " + name + " 0 * :" + name + CRLF);
	msg.append("PASS " + _pw + CRLF);

	sendMessage(msg);
}

void Bot::_joinChannel(const std::string& channel)
{
	sendMessage("JOIN " + channel + CRLF);
}


// -------------------------------------------------------------------------- //
// LEGACY TESTING FUNCTIONS
// -------------------------------------------------------------------------- //



// void Bot::_spamUser(const std::string& nick)
// {
// 	_sendToServer("PRIVMSG " + nick + " :" + SPAM_MSG_42 + CRLF);
// }

// void Bot::_spamChannel(const std::string& channel)
// {
// 	_sendToServer("PRIVMSG " + channel + " :" + SPAM_MSG_CH + CRLF);
// }
