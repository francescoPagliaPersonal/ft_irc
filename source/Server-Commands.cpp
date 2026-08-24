/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server-Commands.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mweghofe <mweghofe@student.42vienna.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/17 16:32:42 by mweghofe          #+#    #+#             */
/*   Updated: 2026/08/24 18:14:10 by mweghofe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"
#include "irc.hpp"

// -------------------------------------------------------------------------- //
// INTERFACE -- COMMANDS
// -------------------------------------------------------------------------- //

// -------------------------------------------------------------------------- //
// PRIVATE -- COMMANDS
// -------------------------------------------------------------------------- //

// Execute all queued messages and handle their protocol errors,
// disconnecting clients whose message handling fails.
void Server::_executeCommands()
{
	rfc numeric = irc::OK;
	bool keep = true;

	if (DEBUG && !_msgsQueue.empty())
		std::cout << "[Info] Processing message queue with "
			<< _msgsQueue.size() << " messages...\n";
	while (!_msgsQueue.empty())
	{
		Message& msg = _msgsQueue.front();
		// TODO ensure POLICY and COMMAND errors are in line with PROTOCOL CODES
		numeric = _cmdReg.execute(*this, msg);
		keep = _cmdReg.handleProtocolErrors(*this, numeric, msg);
		if (!keep)
			_disconnectClient(msg.sender);
		_msgsQueue.pop_front();
	}
}

// Remove all queued messages sent by CLIENT.
void Server::_removeMsgsFromSuspicious(Client *client)
{
	std::deque<Message>::iterator it = _msgsQueue.begin();
	while (it != _msgsQueue.end())
    {
        if (it->sender == client)
            it = _msgsQueue.erase(it); 
        else
            ++it;
    }
}
