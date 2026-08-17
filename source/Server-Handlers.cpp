/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server-Handlers.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mweghofe <mweghofe@student.42vienna.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/09 22:58:08 by mweghofe          #+#    #+#             */
/*   Updated: 2026/08/16 19:08:44 by mweghofe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"
#include "ft_irc.hpp"

#include <cstring>
#include <netinet/in.h>
#include <unistd.h>

// Responsible for handling the process of a new incoming connection.
void Server::_handleListenEvent()
{
	while (true)
	{
		struct sockaddr_in ipAddr; // in case client needs it later
		// 1) create new FD for new connection
		int newFD = _listener.acceptConnection(ipAddr);
		if (newFD < 0)
			return ;
		// 2) check for max clients
		if (_clients.size() >= MAX_CLIENTS)
		{
			::close(newFD); // TODO info msg
			break ; // needs to break, to allow loop to empty queue
		}
		// 3) register new client
		_registerNewClient(newFD, ipAddr);
		std::cout << "[Info] New connection accepted at FD " << newFD << '\n';
	}
}

// Responsible for handling all epoll events on client FDs.
void Server::_handleClientEvent(epoll_event& ev)
{
	e_pollret ret = RET_OK;
	Client* client = static_cast<Client*>(ev.data.ptr);
	if (ev.events & (EPOLLHUP | EPOLLERR))
		ret = RET_CLOSE;
	else if (ev.events & EPOLLIN)
		ret = client->receiveToBuffer();
	else if (ev.events & EPOLLOUT)
		ret = client->sendFromBuffer();
	switch (ret)
	{
		case RET_EMPTY:
			_epoll.mod(client->getFD(), DEF_EPOLL_FL, client);
			break;
		case RET_CLOSE:
			_removeClient(client);
			// TODO closing events needs validation, thus also the printout
			std::cout << "[Warning] " << __FUNCTION__ << " removed a Client." << std::endl;
			// TODO but also, are they the same for IN/OUT?
			break;
		case RET_HASOUTPUT:
			_epoll.mod(client->getFD(), DEF_EPOLL_FL | EPOLLOUT, client);
			break;
		case RET_PARSEINPUT:
			if (_processInputBuffer(client) == false)
				_removeClient(client); // builds the interneal message array
			break;
		default: ;
	}
}
