/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Bot-Operation.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mweghofe <mweghofe@student.42vienna.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/02 09:28:34 by mweghofe          #+#    #+#             */
/*   Updated: 2026/09/08 14:10:44 by mweghofe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Bot.hpp"
#include "ft_irc.hpp"
#include "irc.hpp"

void Bot::run()
{
	while (_keepRunning)
	{
		_waitForServer();
		// register with server and join a default channel
		_registerWith("bot");
		_joinChannel(SPAM_CHANNEL);
		// stay alive in epoll loop
		_epollHandler();	
	}

}

void Bot::sendMessage(const std::string& msg)
{
	(void) msg;
	if (_bufOUT.empty())
		_epoll.mod(_fd, EPOLL_FL_DEFAULT | EPOLLOUT, NULL);
	_bufOUT.append(msg);
	if (DEBUG)
		std::cout << "[Bot] Appending to output buffer:\n" << msg;
}

void Bot::_epollHandler()
{
	struct epoll_event ev;
	while (_keepRunning && _hasConn)
	{
		irc::epollret ret = irc::RET_OK;
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
				_connect();
				break;
			case irc::RET_HASOUTPUT: // FIXME this needs to go completely (also server) send must just RET_OK
				_epoll.mod(_fd, EPOLL_FL_DEFAULT | EPOLLOUT, NULL);
			case irc::RET_PARSEINPUT:
				_processInputBuffer();
				break;
		}
		// TODO have it's own housekeeping & send a ping from time to time?
		// sleep(2);
		// _spamUser(SPAM_USER);
		// _spamChannel(SPAM_CHANNEL);
	}
}
