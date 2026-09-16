/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server-Run.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mweghofe <mweghofe@student.42vienna.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/11 12:06:37 by mweghofe          #+#    #+#             */
/*   Updated: 2026/09/16 14:43:46 by mweghofe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_irc.hpp"
#include "Server.hpp"

// -------------------------------------------------------------------------- //
// MAIN LOOP
// -------------------------------------------------------------------------- //

// Main Server Interface.
// Listening Socket and epoll() registration
// has already be done by the constructor.
void Server::run()
{
	struct epoll_event ev[MAX_EVENTS]; 

	while (_isAlive)
	{
		int readyFDs = _epoll.wait(ev, MAX_EVENTS, TIMEOUT);
		if (readyFDs != 0 && DEBUG && ev->events & EPOLLIN)
			std::cout << std::endl;
		if (DEBUG == debug::DETAILED)
			std::cout << "[Server] Event or timout triggered. Ready FDs: "
					  << readyFDs << std::endl;
		// 1) epoll() stuff
		for (int i = 0; i < readyFDs; i++)
		{
			// A) servers's own listening port
			if (ev[i].data.fd == _listener.getFD())
			{
				_handleListenEvent();
				continue ;
			}
			// B) normal client (EPOLLIN/EPOLLOUT, filling/emptying our buffers)
			_handleClientEvent(ev[i]);
		}
		// 2) work the command queue (execute msg queue, fill client write buff)
		_executeCommands();
		// 3) housekeeping (signal, timeout, sth else?)
		_housekeeping();
	}
}
