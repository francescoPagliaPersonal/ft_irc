/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server-Handlers.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mweghofe <mweghofe@student.42vienna.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/09 22:58:08 by mweghofe          #+#    #+#             */
/*   Updated: 2026/08/12 14:21:06 by mweghofe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"
#include <cstring>
#include <netinet/in.h>
#include <unistd.h>

void Server::_handleListenEvent()
{
	while (true)
	{
		struct sockaddr_in ipAddr; // in case client needs it later
		// 1) create new FD for new connection
		int newFD = _listener._acceptConnection(ipAddr);
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
		if (DEBUG)
		{
			std::cout << __FUNCTION__ << " accepted a new client connection.\n";
			_clients[newFD]->_debugWriteToBuffer("Testing...\nLoading...\n");
			_epoll._mod(newFD, DEF_EPOLL_FL | EPOLLOUT, _clients[newFD]);
		}
	}
}

void Server::_handleClientEvent(epoll_event& ev)
{
	e_pollret ret = RET_OK;
	Client* client = static_cast<Client*>(ev.data.ptr);
	if (ev.events & (EPOLLHUP | EPOLLERR))
		ret = RET_CLOSE;
	else if (ev.events & EPOLLIN)
		ret = client->_receiveToBuffer();
	else if (ev.events & EPOLLOUT)
		ret = client->_sendFromBuffer();
	switch (ret)
	{
		case RET_EMPTY:
			_epoll._mod(client->_getFD(), DEF_EPOLL_FL, client);
			break;
		case RET_CLOSE:
			_removeClient(client);
			std::cout << __FUNCTION__ << " removed a Client." << std::endl;
			// TODO but also, are they the same for IN/OUT?
			break;
		case RET_HASOUTPUT:
			_epoll._mod(client->_getFD(), DEF_EPOLL_FL | EPOLLOUT, client);
			break;
		case RET_CMDTOOLONG: ; // TODO
		case RET_PARSEINPUT:
			_processInputBuffer(); // builds the interneal message array
			break;
		default: ;
	}
}

void Server::_processInputBuffer()
{
	// does a million other things
	// and eventually transformStringToMessage()
}
