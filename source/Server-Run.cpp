/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server-Run.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mweghofe <mweghofe@student.42vienna.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/11 12:06:37 by mweghofe          #+#    #+#             */
/*   Updated: 2026/08/16 18:23:44 by mweghofe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

// -------------------------------------------------------------------------- //
// MAIN LOOP
// -------------------------------------------------------------------------- //

// Main Server Interface.
// Listening Socket and epoll() registration
// has already be done by the constructor.
void Server::run()
{
	struct epoll_event ev[MAX_EVENTS]; // TODO do we need to zero that one?

	while (_isAlive)
	{
		int readyFDs = _epoll.wait(ev, MAX_EVENTS, TIMEOUT);
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
		// TODO devise tasks to do
	}
}

void Server::_executeCommands()
{
	int numeric = 0;
	bool keep = true;

	if (DEBUG && !_msgsQueue.empty())
		std::cout << "[Info] Processing message queue with "
			<< _msgsQueue.size() << " messages...\n";
	while (!_msgsQueue.empty())
	{
		Message& msg = _msgsQueue.front();
		// TODO ensure POLICY and COMMAND errors are in line with PROTOCOL CODES
		numeric = _cmdReg.execute(*this, msg);
		keep = _cmdReg.handleProtocolErrors(*this, numeric, msg);
		if (!keep)
			_disconnectClient(msg.sender);
		_msgsQueue.pop_front();
	}
}
