/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Bot-RunUntilDisconnect.cpp                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mweghofe <mweghofe@student.42vienna.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/02 09:28:34 by mweghofe          #+#    #+#             */
/*   Updated: 2026/09/18 12:22:35 by mweghofe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Bot.hpp"
#include "ft_irc.hpp"
#include "irc.hpp"

#include <cstring>

void Bot::_runUntilDisconnect()
{
	struct epoll_event ev;
	irc::epollret ret = irc::RET_OK;

	std::memset(&ev, 0, sizeof(ev));
	bool joinedDefChan = false;
	// 1) new connection: register on server
	_registerWith(BOT_NAME);

	while (_keepRunning && _hasConn)
	{
		// 2) finish server setup
		if (!joinedDefChan && _joinedServer)
		{
			_joinChannel(DEFAULT_CHANNEL);
			_lockAndSetTopic(DEFAULT_TOPIC);
			joinedDefChan = true;
		}
		
		// 3) retrieve epoll events or wake up on timeout
		int ready = _epoll.wait(&ev, 1, TIMEOUT);

		// 4) take action on event
		if (ready > 0)
		{
			if (ev.events & (EPOLLHUP | EPOLLERR))
				ret = irc::RET_CLOSE;
			else if (ev.events & EPOLLIN)
				ret = _receiveToBuffer();
			else if (ev.events & EPOLLOUT)
				ret = _sendFromBuffer();
		}
		// 5) pick followup task
		switch (ret)
		{
			case irc::RET_OK:
				break;
			case irc::RET_EMPTY:
				_epoll.mod(_fd, EPOLL_FL_DEFAULT, NULL);
				break;
			case irc::RET_CLOSE:
				_hasConn = false;
				std::cout << "[Bot] (" << irc::timeNowStr() << ") Connection to server lost.\n";
				break;
			case irc::RET_PARSEINPUT:
				_processInputBuffer();
				break;
		}
		// 6) custom commands and internal actions
		_executeMessages();
	}
}
