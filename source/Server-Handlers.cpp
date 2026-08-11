/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server-Handlers.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mweghofe <mweghofe@student.42vienna.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/09 22:58:08 by mweghofe          #+#    #+#             */
/*   Updated: 2026/08/11 14:33:54 by mweghofe         ###   ########.fr       */
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
		struct sockaddr_in ipAddr; // in case client needs it later
		// 1) create new FD for new connection
		int newFD = listener_.acceptConnection(ipAddr);
		if (newFD < 0)
			return ;
		// 2) check for max clients
		if (clients_.size() >= MAX_CLIENTS)
		{
			::close(newFD); // TODO info msg
			break ; // needs to break, to allow loop to empty queue
		}
		// 3) register new client
		registerNewClient(newFD, ipAddr);
		if (DEBUG)
			std::cout << __FUNCTION__ << " accepted a new client connection.\n";
	}
}

void Server::handleClientEvent(epoll_event& ev)
{
	e_pollret ret = RET_OK;
	Client* client = static_cast<Client*>(ev.data.ptr);

	if (ev.events & (EPOLLHUP | EPOLLERR))
		ret = RET_ERROR; // TODO perhaps frame this as RET_CLOSE?
	else if (ev.events & EPOLLIN)
		ret = client->receiveToBuffer();
	else if (ev.events & EPOLLOUT)
		ret = client->sendFromBuffer();
	switch (ret)
	{
		case RET_EMPTY:
			epoll_.mod(ev.data.fd, DEF_EPOLL_FL, client);
			break;
		case RET_ERROR:
			removeClient(client);
			// TODO but also, are they the same for IN/OUT?
			// TODO perhaps frame this as RET_CLOSE?
			break;
		case RET_HASOUTPUT:
			epoll_.mod(ev.data.fd, DEF_EPOLL_FL | EPOLLOUT, client);
			break;
		case RET_CMDTOOLONG: ; // TODO 
		default: ;
	}
}
