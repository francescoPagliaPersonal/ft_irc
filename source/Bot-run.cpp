/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Bot-run.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mweghofe <mweghofe@student.42vienna.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/02 09:28:34 by mweghofe          #+#    #+#             */
/*   Updated: 2026/09/07 16:49:39 by mweghofe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Bot.hpp"
#include "ft_irc.hpp"
#include "irc.hpp"

#include <sys/socket.h>
#include <sys/epoll.h>

#include <cerrno>
#include <cstring>
#include <iostream>
#include <stdexcept>

#include <unistd.h> // sleep()

void Bot::run()
{
	while (_keepRunning)
	{
		// Retry failed connections with a fresh socket until connection is made.
		// 1) wait until kernel finishes handshake
		//    NOTE: epoll returns ready==1 on BOTH success and failure.
		//    EPOLLOUT on success, EPOLLERR|EPOLLHUP on failure both wake wait().
		//    ready>0 only means handshake finished, NOT that it succeeded.
		//    Must check SO_ERROR next to distinguish.
		// 2) after handshake ask kernel for result
		// 3) if there was no connection, wait a bit and try again
		while (_keepRunning && !_hasConn)
		{
			int ready = _awaitHandshake();
			if (ready == 0)
				continue;
			if (_handshakeResult() == 0)
				break;
			sleep(3);
			_connect();
		}
		if (_keepRunning)
		{
			// 3) register with server and join a default channel
			_registerWith("bot");
			_joinChannel(SPAM_CHANNEL);
		}
		
		// 4) Stay alive in epoll loop
		struct epoll_event ev;
		while (_keepRunning && _hasConn)
		{
			irc::epollret ret = irc::RET_OK;
			int ready = _epoll.wait(&ev, 1, TIMEOUT);
			if (ready > 0)
			{
				if (ev.events & (EPOLLHUP | EPOLLERR))
					ret = irc::RET_CLOSE;
				else if (ev.events & EPOLLIN)
					ret = _discardInput(); // TODO proper read
				else if (ev.events & EPOLLOUT)
					; // TODO OUT
			}
			switch (ret)
			{
				case irc::RET_EMPTY:
					_epoll.mod(_fd, EPOLL_FL_DEFAULT, NULL);
					break;
				case irc::RET_CLOSE:
					_hasConn = false;
					std::cout << "[Bot] Connection to server lost.\n";
					// HACK i don't like this... might need to separate cleanup & connect
					_connect();
					break;
				case irc::RET_HASOUTPUT:
					_epoll.mod(_fd, EPOLL_FL_DEFAULT | EPOLLOUT, NULL);
				case irc::RET_PARSEINPUT:
					_processInputBuffer();
					break;
				default: ;
			}
			// TODO send a ping from time to time?
			// sleep(2);
			// _spamUser(SPAM_USER);
			// _spamChannel(SPAM_CHANNEL);
		}
	}

}
