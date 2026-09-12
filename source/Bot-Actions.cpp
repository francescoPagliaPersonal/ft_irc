/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Bot-Actions.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mweghofe <mweghofe@student.42vienna.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/02 08:12:43 by mweghofe          #+#    #+#             */
/*   Updated: 2026/09/12 15:07:02 by mweghofe         ###   ########.fr       */
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
// PUBLIC ACTIONS
// -------------------------------------------------------------------------- //

void Bot::toggleMirror()
{
	_mirrorMsg = !_mirrorMsg;
}

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
// PRIVATE ACTIONS
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

// Mirrors any message that the bot receives via PRIVMSG
void Bot::_mirrorMessage(const Message& msg)
{
	if (msg.params.empty())
		return ;
	// 1) Channel Replies
	if (msg.params[0][0] == '#' || msg.params[0][0] == '&')
		sendMessage(std::string("PRIVMSG ") + msg.params[0] + " :" + msg.trailing + CRLF );
	// 2) Private Message Reply
	else if (msg.params[0] == BOT_NAME)
	{
		if (msg.prefix.empty())
			return ;
		std::string sender = msg.prefix.substr(0, msg.prefix.find('!'));
		sendMessage(std::string("PRIVMSG ") + sender + " :" + msg.trailing + CRLF );
	}
}

void Bot::_processInvite(const Message& msg)
{
	if (msg.params.size() < 2)
		return ;
	if (msg.params[0] != BOT_NAME)
		return ;
	_joinChannel(msg.params[1]);
}

void Bot::_pong(const Message& msg)
{
	if (msg.params.empty())
		return ;
	if (msg.params[0] != BOT_NAME)
		return ;
	sendMessage("PONG " + std::string(BOT_NAME) + CRLF);
}
