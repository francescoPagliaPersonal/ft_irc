/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server-Run.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mweghofe <mweghofe@student.42vienna.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/11 12:06:37 by mweghofe          #+#    #+#             */
/*   Updated: 2026/08/11 13:00:00 by mweghofe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

// -------------------------------------------------------------------------- //
// MAIN LOOP
// -------------------------------------------------------------------------- //

void Server::run()
{
	struct epoll_event ev[MAX_EVENTS]; // TODO do we need to zero that one?

	while (isAlive_)
	{
		int readyFDs = epoll_.wait(ev, MAX_EVENTS, TIMEOUT);
		// 1) epoll() stuff
		for (int i = 0; i < readyFDs; i++)
		{
			const int fd = ev[i].data.fd;
			// A) servers's own listening port
			if (fd == listener_.getFD())
			{
				handleListenEvent();
				continue ;
			}
			// B) normal client (EPOLLIN/EPOLLOUT, filling/emptying our buffers)
			handleClientEvent(ev[i]);
		}
		// 2) work the command queue (execute read buff, create write buff)
		// 3) housekeeping (signal, timeout, sth else?)
	}
}
