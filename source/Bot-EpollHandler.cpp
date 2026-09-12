/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Bot-EpollHandler.cpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mweghofe <mweghofe@student.42vienna.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/02 09:28:34 by mweghofe          #+#    #+#             */
/*   Updated: 2026/09/12 09:10:22 by mweghofe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Bot.hpp"
#include "ft_irc.hpp"
#include "irc.hpp"
#include "CommandRegistry.hpp"
#include <cstring>
#include <unistd.h>

void Bot::_runUntilDisconnect()
{
	struct epoll_event ev;
	irc::epollret ret = irc::RET_OK;

	std::memset(&ev, 0, sizeof(ev));
	bool joinedDefChan = false;
	_registerWith(BOT_NAME);

	while (_keepRunning && _hasConn)
	{
		if (!joinedDefChan && _joinedServer)
		{
			_joinChannel(DEFAULT_CHANNEL);
			joinedDefChan = true;
		}
		
		// retrieve epoll events or wake up on timeout
		int ready = _epoll.wait(&ev, 1, TIMEOUT);

		// take action on event
		if (ready > 0)
		{
			if (ev.events & (EPOLLHUP | EPOLLERR))
				ret = irc::RET_CLOSE;
			else if (ev.events & EPOLLIN)
				ret = _receiveToBuffer();
			else if (ev.events & EPOLLOUT)
				ret = _sendFromBuffer();
		}
		// pick followup task
		switch (ret)
		{
			case irc::RET_OK:
				break;
			case irc::RET_EMPTY:
				_epoll.mod(_fd, EPOLL_FL_DEFAULT, NULL);
				break;
			case irc::RET_CLOSE:
				_hasConn = false;
				std::cout << "[Bot] Connection to server lost.\n";
				break;
			case irc::RET_PARSEINPUT:
				_processInputBuffer();
				break;
			default: ; // HACK because there is still HASOUTPUT present in header
		}
		_executeMessages();
	}
}

// simple startup of the command execution.
void Bot::_executeMessages()
{
	if (DEBUG && !_msgsQueue.empty())
		std::cout << "[Info] Processing message queue with "
			<< _msgsQueue.size() << " messages...\n";
	while (!_msgsQueue.empty())
	{
		Message& msg = _msgsQueue.front();
		std::vector<std::string> botcmds = irc::strSplit(msg.trailing, ' ', false);
		if (!_cmdReg.execute(*this, msg, botcmds))
		{
			if (msg.command == "001")
				_joinedServer = true;
			else if (msg.command == "433")
			{
				_keepRunning = false;
				std::cout << "[Bot] Another bot is already connected.\n";
			}
			else if (msg.command == "JOIN"
				&& msg.prefix.find(std::string(":") + BOT_NAME) != msg.prefix.npos
				&& msg.params[0] == DEFAULT_CHANNEL)
				_joinDefChan =  true;
			else if (msg.command == "PRIVMSG"
				&& msg.prefix.find(std::string(":") + BOT_NAME) == msg.prefix.npos
				&& msg.params[0] == DEFAULT_CHANNEL)
				sendMessage(std::string("PRIVMSG ") + DEFAULT_CHANNEL + " :" + msg.trailing + CRLF ); 
		}
		_msgsQueue.pop_front();
	}
}
