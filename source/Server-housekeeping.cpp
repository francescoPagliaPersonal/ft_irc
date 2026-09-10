/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server-housekeeping.cpp                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mweghofe <mweghofe@student.42vienna.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/03 11:38:33 by mweghofe          #+#    #+#             */
/*   Updated: 2026/09/10 17:41:55 by mweghofe         ###   ########.fr       */
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
	static std::time_t lastPing = std::time(NULL);
	std::time_t now = std::time(NULL);
	// -- remove conspicuous clients
	while (_toRemove.size())
	{
		Client* client = *(_toRemove.begin());
		_toRemove.erase(client);
		_deleteClient(client);
	}
	// -- send PING
	if (std::difftime(now, lastPing) >= INTERVAL_PING)
	{
		std::map<int, Client*>::iterator it;
		std::time_t minLast = now - INTERVAL_PING;

		// --- TESTING ---
		char buffer[9];
		std::strftime(buffer, sizeof(buffer), "%H:%M:%S", std::localtime(&now));
		std::string strNow(buffer);
		std::strftime(buffer, sizeof(buffer), "%H:%M:%S", std::localtime(&minLast));
		std::string strLast(buffer);
		std::cout << " -- ping must be checked -- "
			<< " (now: " << strNow <<" | ping threshold: " << strLast << ")\n";
		
		for (it  = _clients.begin(); it != _clients.end(); it++)
		{
			Client* client = it->second;

			// --- TESTING ---
			std::time_t clT = client->getLastMsgTime();
			std::strftime(buffer, sizeof(buffer), "%H:%M:%S", std::localtime(&clT));
			std::string strC(buffer);
			
			if (client->hasQuit() || client->getLastMsgTime() > minLast)
			{
				std::cout << "\tFD " << client->getFD() << " last msg @ " << strC << '\n';
				continue ;
			}

			std::cout << "\tFD " << client->getFD() << " last msg @ " << strC << " ...sending\n";

			if (client->getPingCount() >= MAX_UNANSWERED_PING)
			{
				_toRemove.insert(client);
				continue ;
			}
			sendMessage(client, "PING " + client->getNick() + CRLF);
			client->incrementPingCount();
		}
		// -- remove conspicuous clients
		while (_toRemove.size())
		{
			Client* client = *(_toRemove.begin());
			_toRemove.erase(client);
			_deleteClient(client);
		}
		lastPing = now;
	}
}
