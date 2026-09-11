/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Bot-EpollHandler.cpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fpaglia <fpaglia@student.42vienna.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/02 09:28:34 by mweghofe          #+#    #+#             */
/*   Updated: 2026/09/11 17:29:53 by fpaglia          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Bot.hpp"
#include "ft_irc.hpp"
#include "irc.hpp"
#include <cstring>
#include <unistd.h>

void Bot::_epollHandler()
{
	struct epoll_event ev;
	irc::epollret ret = irc::RET_OK;

	std::memset(&ev, 0, sizeof(ev));
	bool loginRequested = false;
	bool defChanJoined = false;
	
	while (_keepRunning && _hasConn)
	{
		if (!loginRequested)
		{
			_registerWith(BOT_NAME);
			loginRequested = true;
		}
		if (_joinSrv && !defChanJoined)
		{
			_joinChannel(DEFAULT_CHANNEL);
			defChanJoined = true;
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
				// HACK i don't like this... might need to separate cleanup & connect
				break;
			case irc::RET_HASOUTPUT: // FIXME this needs to go completely (also server) send must just RET_OK
				_epoll.mod(_fd, EPOLL_FL_DEFAULT | EPOLLOUT, NULL);
			case irc::RET_PARSEINPUT:
				_processInputBuffer();
				break;
		}
		_executeMessage();
	}
}

// simple startup of the command execution.
void Bot::_executeMessage()
{
	if (DEBUG && !_msgsQueue.empty())
		std::cout << "[Info] Processing message queue with "
			<< _msgsQueue.size() << " messages...\n";
	while (!_msgsQueue.empty())
	{
		Message& msg = _msgsQueue.front();
		
		if (msg.command == "001")
			_joinSrv = true;
		else if (msg.command == "433")
			_keepRunning = false;	
		else if (msg.prefix.find(std::string(":") + BOT_NAME) != msg.prefix.npos 
			&& msg.command == "JOIN"
			&& msg.params[0] == DEFAULT_CHANNEL)
			_joinDefChan =  true;
		else if (msg.prefix.find(std::string(":") + BOT_NAME) == msg.prefix.npos
			&& msg.command == "PRIVMSG"
			&& msg.params[0] == DEFAULT_CHANNEL)
			sendMessage(std::string("PRIVMSG ") + DEFAULT_CHANNEL + " :" + msg.trailing + CRLF ); 
		_msgsQueue.pop_front();
	}
}
