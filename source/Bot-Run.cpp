/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Bot-Run.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mweghofe <mweghofe@student.42vienna.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/11 17:04:38 by fpaglia           #+#    #+#             */
/*   Updated: 2026/09/11 20:37:07 by mweghofe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Bot.hpp"
#include "ft_irc.hpp"

#include <cstring>

// Main Bot Interface.
// Keeps the bot alive whether or not a connection to the server is established.
void Bot::run()
{
	int secDelay = 5;
	int attempt = 3;
	
	while (_keepRunning)
	{
		// attempt to establish a conneciton the server
		_fd = connectWithRetry(attempt, secDelay);
		if (_fd == -1)
		{
			secDelay = secDelay <= 640 ? secDelay *2 : secDelay;
			continue ;	
		}
		std::cout << "[Bot] Connection to server established (FD " << _fd << ").\n";
		// stay alive main loop: register, join, epoll events
		_runUntilDisconnect();
		// clean up a lost connection, before attempting to reconnect
		_bufIN.clear();
		_bufOUT.clear();
		if (_fd != -1) {
            _epoll.del(_fd);
            ::close(_fd);
            _fd = -1;
        }
	}

}
