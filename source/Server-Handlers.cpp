/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server-Handlers.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mweghofe <mweghofe@student.42vienna.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/09 22:58:08 by mweghofe          #+#    #+#             */
/*   Updated: 2026/08/11 13:05:27 by mweghofe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"
#include <cstring>
#include <netinet/in.h>
#include <unistd.h>

void Server::handleListenEvent()
{
	while (true)
	{
		std::cout << __FUNCTION__ << " was triggered.\n"; // TODO remove
		struct sockaddr_in ipAddr; // in case client needs it later
		// 1) create new FD for new connection
		int newFD = listener_.acceptConnection(ipAddr);
		if (newFD < 0)
			return ;
		// 2) check for max clients
		if (clients_.size() > MAX_CLIENTS)
		{
			::close(newFD); // TODO info msg
			break ; // needs to break, to allow loop to empty queue
		}
		// 3) register new client
		clients_[newFD] = Client::registerNew(newFD, ipAddr, nextClient_);
		nextClient_++;
	}
}

void Server::handleClientEvent(epoll_event& ev)
{
	e_pollret ret;
	Client* client = static_cast<Client*>(ev.data.ptr);

	if (ev.events & EPOLLIN)
	{
		ret = client->receiveToBuffer();
	}
	else if (ev.events & EPOLLOUT)
	{
		ret = client->sendFromBuffer();
	}
	switch (ret)
	{
		case RET_EMPTY:
			epoll_.mod(ev.data.fd, DEF_EPOLL_FL, client);
			break;
		case RET_ERROR: // TODO but also, are they the same for IN/OUT?
			break;
		case RET_HASOUTPUT:
			epoll_.mod(ev.data.fd, DEF_EPOLL_FL | EPOLLOUT, client);
			break;
		case RET_CMDTOOLONG: ; // TODO 
		default: ;
	}
}
