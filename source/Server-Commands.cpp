/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server-Commands.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mweghofe <mweghofe@student.42vienna.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/17 16:32:42 by mweghofe          #+#    #+#             */
/*   Updated: 2026/08/28 07:59:27 by mweghofe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_irc.hpp"
#include "Server.hpp"
#include "irc.hpp"
#include "Response.hpp"

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
	rfc numeric;
	if (DEBUG && !_msgsQueue.empty())
		std::cout << "[Info] Processing message queue with "
			<< _msgsQueue.size() << " messages...\n";
	// process every command in the message queue
	while (!_msgsQueue.empty())
	{
		Message& msg = _msgsQueue.front();
		// TODO ensure POLICY and COMMAND errors are in line with PROTOCOL CODES
		numeric = _cmdReg.execute(*this, msg);
		_msgsQueue.pop_front();
		if (numeric == irc::HASQUIT)
			_prepareClientDisconnect(msg.sender);
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
