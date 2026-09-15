/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server-housekeeping.cpp                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mweghofe <mweghofe@student.42vienna.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/03 11:38:33 by mweghofe          #+#    #+#             */
/*   Updated: 2026/09/15 15:52:25 by mweghofe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Client.hpp"
#include "Server.hpp"
#include "irc.hpp"
#include "ft_irc.hpp"

#include <map>

// Inserts a CLIENT that is to be removed into the set of clients to be removed.
void Server::_addToRemove(Client* client) const // with the mutable attribute it has to be const
{
	_toRemove.insert(client);
	client->setRemove(true);
}

// Periodically checks for clients to be removed and sends PING to clients
void Server::_housekeeping()
{
	// 0) prepare current time
	static std::time_t lastPing = std::time(NULL);
	std::time_t now = std::time(NULL);
	// 1) remove conspicuous clients
	_removeMarkedClients();
	// 2) send PING
	if (std::difftime(now, lastPing) >= INTERVAL_PING)
	{
		// a) prepare timer for clients to skip
		std::time_t inactivityThreshold = now - INTERVAL_PING;
		if (DEBUG == debug::DETAILED)
			std::cout << "[Server] Current Time is " << irc::timeAsStr(now) 
				<<". PINGing clients who's last activity was recorded before "
				<< irc::timeAsStr(inactivityThreshold) << '\n';
		// b) check all the clients
		std::map<int, Client*>::iterator it;
		for (it  = _clients.begin(); it != _clients.end(); it++)
		{
			_clientPingSkipRemove(it->second, inactivityThreshold);
		}
		// c) remove clients that haven't responded and update ping time
		_removeMarkedClients();
		lastPing = now;
	}
}

// Remove all clients that have been marked for removal
void Server::_removeMarkedClients()
{
	while (_toRemove.size())
	{
		if (DEBUG == debug::DETAILED)
			std::cout << "[Server] Removing marked clients...\n";
		Client* client = *(_toRemove.begin());
		_toRemove.erase(client);
		_deleteClient(client);
	}
}

void Server::_clientPingSkipRemove(Client* client, std::time_t inactivityThreshold)
{
	// 1) skip clients that have been active within the past interval
	if (client->hasQuit() || client->getLastMsgTime() >= inactivityThreshold)
	{
		if (DEBUG == debug::DETAILED)
			std::cout << "\t FD " << client->getFD() << " last msg @ "
				<< irc::timeAsStr(client->getLastMsgTime()) << '\n';
		return ;
	}
	if (DEBUG == debug::DETAILED)
		std::cout << "\t FD " << client->getFD() << " last msg @ "
			<< irc::timeAsStr(client->getLastMsgTime()) << " ...sending\n";
	// 2) mark clients that haven't responded to pings for removal
	if (client->getPingCount() >= MAX_UNANSWERED_PING)
	{
		_toRemove.insert(client);
		return ;
	}
	// 3) send a new PING and count
	sendMessage(client, "PING " + client->getNick() + CRLF);
	client->incrementPingCount();
}
