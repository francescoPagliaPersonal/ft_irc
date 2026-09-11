/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server-housekeeping.cpp                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mweghofe <mweghofe@student.42vienna.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/03 11:38:33 by mweghofe          #+#    #+#             */
/*   Updated: 2026/09/11 14:48:15 by mweghofe         ###   ########.fr       */
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
		if (DEBUG == debug::DETAILED)
			std::cout << "[Server] Removing clients...\n";
		Client* client = *(_toRemove.begin());
		_toRemove.erase(client);
		_deleteClient(client);
	}
	// -- send PING
	if (std::difftime(now, lastPing) >= INTERVAL_PING)
	{
		std::map<int, Client*>::iterator it;
		std::time_t minLast = now - INTERVAL_PING;

		if (DEBUG == debug::DETAILED)
			std::cout << "[Server] Time to PING inactive clients,"
				<< " last active before "
				<< irc::timeAsStr(minLast) << " (now " 
				<< irc::timeAsStr(now) << ")\n";
				// << " (now: " << irc::timeAsStr(now)
				// << " | ping threshold: " << irc::timeAsStr(minLast) << ")\n";
		
		for (it  = _clients.begin(); it != _clients.end(); it++)
		{
			Client* client = it->second;
			
			if (client->hasQuit() || client->getLastMsgTime() >= minLast)
			{
				if (DEBUG == debug::DETAILED)
					std::cout << "\t FD " << client->getFD() << " last msg @ "
						<< irc::timeAsStr(client->getLastMsgTime()) << '\n';
				continue ;
			}

			if (DEBUG == debug::DETAILED)
				std::cout << "\t FD " << client->getFD() << " last msg @ "
					<< irc::timeAsStr(client->getLastMsgTime()) << " ...sending\n";

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
