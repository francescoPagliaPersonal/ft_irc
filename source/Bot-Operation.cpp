/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Bot-Operation.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fpaglia <fpaglia@student.42vienna.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/02 09:28:34 by mweghofe          #+#    #+#             */
/*   Updated: 2026/09/10 15:57:44 by fpaglia          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Bot.hpp"
#include "ft_irc.hpp"
#include "irc.hpp"
#include <unistd.h>



void Bot::run()
{
	int delay = 3;
	while (_keepRunning)
	{
		_fd = connectWithRetry(3, delay);
		if (_fd == -1)
		{
			delay = delay <= 640 ? delay *2 : delay;
			continue ;	
		}
		std::cout << "[bot] registered with fd: " << _fd << std::endl;
		_epoll.add(_fd, EPOLL_FL_DEFAULT);

		_registerWith("bot");
		_joinChannel(DEFAULT_CHANNEL);
		// stay alive in epoll loop
		_epollHandler();	
		_bufIN.clear();
		_bufOUT.clear();
		if (_fd != -1) {
            _epoll.del(_fd);
            ::close(_fd);
            _fd = -1;
        }
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
	irc::epollret ret = irc::RET_OK;

	while (_keepRunning && _hasConn)
	{
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
		// TODO have it's own housekeeping & send a ping from time to time?
		// sleep(2);
		// _spamUser(SPAM_USER);
		// _spamChannel(SPAM_CHANNEL);
	}
}
