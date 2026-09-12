/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Bot-Actions.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mweghofe <mweghofe@student.42vienna.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/02 08:12:43 by mweghofe          #+#    #+#             */
/*   Updated: 2026/09/12 09:44:20 by mweghofe         ###   ########.fr       */
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

void Bot::_mirrorMessage(const Message& msg)
{
	if (msg.prefix.find(std::string(":") + BOT_NAME) == msg.prefix.npos
		&& msg.params[0] == DEFAULT_CHANNEL)
	{
		sendMessage(std::string("PRIVMSG ") + DEFAULT_CHANNEL + " :" + msg.trailing + CRLF );
	}
}
