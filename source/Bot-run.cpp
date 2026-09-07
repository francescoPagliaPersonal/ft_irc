/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Bot-run.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mweghofe <mweghofe@student.42vienna.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/02 09:28:34 by mweghofe          #+#    #+#             */
/*   Updated: 2026/09/07 13:05:56 by mweghofe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Bot.hpp"
#include "ft_irc.hpp"

#include <sys/socket.h>
#include <sys/epoll.h>

#include <cerrno>
#include <cstring>
#include <iostream>
#include <stdexcept>

#include <unistd.h> // sleep()

void Bot::run()
{
	while (_isAlive)
	{
		// Retry failed connections with a fresh socket until connection is made.
		// 1) wait until kernel finishes handshake
		//    NOTE: epoll returns ready==1 on BOTH success and failure.
		//    EPOLLOUT on success, EPOLLERR|EPOLLHUP on failure both wake wait().
		//    ready>0 only means handshake finished, NOT that it succeeded.
		//    Must check SO_ERROR next to distinguish.
		// 2) after handshake ask kernel for result
		// 3) if there was no connection, wait a bit and try again
		while (!_hasConn)
		{
			int ready = _awaitHandshake();
			if (ready == 0)
				continue;
			if (_handshakeResult() == 0)
				break;
			sleep(3);
			_connect();
		}
		// 3) register with server and join a default channel
		_registerWith("bot");
		_joinChannel(SPAM_CHANNEL);
		// 4) Stay alive with dummy loop
		while (_hasConn)
		{
			// sleep(2);
			_spamUser(SPAM_USER);
			_spamChannel(SPAM_CHANNEL);
		}
	}

}
