/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server-housekeeping.cpp                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mweghofe <mweghofe@student.42vienna.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/03 11:38:33 by mweghofe          #+#    #+#             */
/*   Updated: 2026/09/05 16:17:45 by mweghofe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Client.hpp"
#include "Server.hpp"
#include "irc.hpp"
#include "ft_irc.hpp"

#include <map>

// Periodically checks for clients to be removed and sends PING to clients
void Server::_housekeeping()
{
	// -- prepare current time
	static std::time_t lastRemoval = std::time(NULL);
	static std::time_t lastPing = std::time(NULL);
	std::time_t now = std::time(NULL);
	// -- remove conspicuous clients
	if (std::difftime(now, lastRemoval) >= INTERVAL_REMOVE)
	{
		while (_toRemove.size())
		{
			Client* client = *(_toRemove.begin());
			_toRemove.erase(client);
			_deleteClient(client);
		}
		lastRemoval = now;
	}
	// -- send PING
	if (std::difftime(now, lastPing) >= INTERVAL_PING)
	{
		std::map<int, Client*>::iterator it;
		for (it  = _clients.begin(); it != _clients.end(); it++)
		{
			Client* client = it->second;
			sendMessage(client, "PING " + client->getNick() + CRLF);
		}
		lastPing = now;
	}
}
