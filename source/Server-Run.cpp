/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server-Run.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mweghofe <mweghofe@student.42vienna.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/11 12:06:37 by mweghofe          #+#    #+#             */
/*   Updated: 2026/08/16 12:42:54 by mweghofe         ###   ########.fr       */
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
		// 2) work the command queue (execute read buff, create write buff)
		_executeCommands(); // TODO uses some container of messages & clients; executes message array
		// 3) housekeeping (signal, timeout, sth else?)
	}
}

void Server::_executeCommands()
{
	while (!_msgsQueue.empty())
	{
		std::cout << "[FD " << _msgsQueue.front().sender->getFD()
			<< "] Processing message queue...\n";
		// TODO refactor and bring as much SERVER control surface here
		// at least epoll_ctl can be done here => nope, new interface
		_cmdReg.execute(*this, _msgsQueue.front());
		// TODO create a PROTOCOL ERROR numeric reply handler in CmdReg or Server
		// TODO   this sends replies AND retuns a keep/drop info 
		// TODO ensure POLICY and PROTOCOL errors are clearly separated and respect the process
		_msgsQueue.pop_front();
	}
}
