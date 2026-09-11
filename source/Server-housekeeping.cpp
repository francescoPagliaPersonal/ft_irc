/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server-housekeeping.cpp                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mweghofe <mweghofe@student.42vienna.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/03 11:38:33 by mweghofe          #+#    #+#             */
/*   Updated: 2026/09/11 15:06:21 by mweghofe         ###   ########.fr       */
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
	// 0) prepare current time
	static std::time_t lastPing = std::time(NULL);
	std::time_t now = std::time(NULL);
	// 1) remove conspicuous clients
	while (_toRemove.size())
	{
		if (DEBUG == debug::DETAILED)
			std::cout << "[Server] Removing clients...\n";
		Client* client = *(_toRemove.begin());
		_toRemove.erase(client);
		_deleteClient(client);
	}
	// 2) send PING
	if (std::difftime(now, lastPing) >= INTERVAL_PING)
	{
		// b) prepare timer for clients to skip
		std::time_t minLast = now - INTERVAL_PING;
		// -- informative debug output
		if (DEBUG == debug::DETAILED)
			std::cout << "[Server] Time to PING inactive clients,"
				<< " last active before "
				<< irc::timeAsStr(minLast) << " (now " 
				<< irc::timeAsStr(now) << ")\n";
				// << " (now: " << irc::timeAsStr(now)
				// << " | ping threshold: " << irc::timeAsStr(minLast) << ")\n";
		// a) 
		std::map<int, Client*>::iterator it;
		for (it  = _clients.begin(); it != _clients.end(); it++)
		{
			Client* client = it->second;
			// c) skip clients that have been active within the past interval
			if (client->hasQuit() || client->getLastMsgTime() >= minLast)
			{
				// -- informative debug output
				if (DEBUG == debug::DETAILED)
					std::cout << "\t FD " << client->getFD() << " last msg @ "
						<< irc::timeAsStr(client->getLastMsgTime()) << '\n';
				continue ;
			}
			// -- informative debug output
			if (DEBUG == debug::DETAILED)
				std::cout << "\t FD " << client->getFD() << " last msg @ "
					<< irc::timeAsStr(client->getLastMsgTime()) << " ...sending\n";
			// b) mark clients that haven't responded to pings for removal
			if (client->getPingCount() >= MAX_UNANSWERED_PING)
			{
				_toRemove.insert(client);
				continue ;
			}
			// c) send a new PING and count
			sendMessage(client, "PING " + client->getNick() + CRLF);
			client->incrementPingCount();
		}
		// d) remove clients that haven't responded
		while (_toRemove.size())
		{
			Client* client = *(_toRemove.begin());
			_toRemove.erase(client);
			_deleteClient(client);
		}
		lastPing = now;
	}
}
