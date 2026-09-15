/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Bot-Run.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mweghofe <mweghofe@student.42vienna.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/11 17:04:38 by fpaglia           #+#    #+#             */
/*   Updated: 2026/09/12 09:34:28 by mweghofe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Bot.hpp"
#include "ft_irc.hpp"

#include <cstring>

// Main Bot Interface.
// Keeps the bot alive whether or not a connection to the server is established.
void Bot::run()
{
	int secDelay = CONN_DELAY;
	
	while (_keepRunning)
	{
		// attempt to establish a conneciton the server
		_fd = _connectWithRetry(secDelay);
		if (_fd == -1)
		{
			secDelay = secDelay <= CONN_MAX_DELAY ? secDelay * 2 : secDelay;
			continue ;	
		}
		std::cout << "[Bot] Connection to server established (FD " << _fd << ").\n";
		// stay alive main loop: register, join, epoll events
		_runUntilDisconnect();
		// clean up a lost connection, before attempting to reconnect
		_joinedServer = false;
		_bufIN.clear();
		_bufOUT.clear();
		if (_fd != -1) {
            _epoll.del(_fd);
            ::close(_fd);
            _fd = -1;
        }
	}
	std::cout << "[Bot] Shutting down.\n";
}
