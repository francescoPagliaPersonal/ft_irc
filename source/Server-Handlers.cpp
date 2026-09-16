/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server-Handlers.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mweghofe <mweghofe@student.42vienna.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/09 22:58:08 by mweghofe          #+#    #+#             */
/*   Updated: 2026/09/16 16:37:47 by mweghofe         ###   ########.fr       */
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
		if (newFD == ListeningSocket::ACCEPT_NONE)
			return ;
		else if (newFD == ListeningSocket::ACCEPT_SETUP_FAIL)
			continue ;
		// 2) check for max clients
		if (_clients.size() >= _maxClients)
		{
			// cannot send a reply due to subject constrains
			// std::string reply = "ERROR: too many connection. ";
			// send(newFD, reply.c_str(), reply.size(), 0);
			::close(newFD);
			continue ;
		}
		// 3) register new client
		_registerNewClient(newFD, ipAddr);
	}
}

// Responsible for handling all epoll events on client FDs.
void Server::_handleClientEvent(epoll_event& ev)
{
	irc::epollret ret = irc::RET_OK;
	Client* client = static_cast<Client*>(ev.data.ptr);
	if (ev.events & (EPOLLHUP | EPOLLERR))
		ret = irc::RET_CLOSE;
	else if (ev.events & EPOLLIN)
		ret = client->receiveToBuffer();
	else if (ev.events & EPOLLOUT)
		ret = client->sendFromBuffer();
	switch (ret)
	{
		case irc::RET_EMPTY:
			if (!client->hasQuit())
				_epoll.mod(client->getFD(), EPOLL_FL_DEFAULT, client);
			else
				_epoll.mod(client->getFD(), EPOLLERR | EPOLLHUP, client);
			break;
		case irc::RET_CLOSE: // this is a forceful disconnect, never a QUIT
			_addToRemove(client);
			break;
		case irc::RET_PARSEINPUT: // builds the interneal message array
			if (_processInputBuffer(client) == irc::RET_CLOSE)
				_addToRemove(client);
			break;
		default: ;
	}
}
