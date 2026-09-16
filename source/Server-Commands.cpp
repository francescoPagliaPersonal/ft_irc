/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server-Commands.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mweghofe <mweghofe@student.42vienna.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/17 16:32:42 by mweghofe          #+#    #+#             */
/*   Updated: 2026/09/16 14:41:26 by mweghofe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_irc.hpp"
#include "Server.hpp"
#include "irc.hpp"
#include "Response.hpp"
#include "Client.hpp"

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
	std::time_t now = time(NULL);
	if (DEBUG && !_msgsQueue.empty())
		std::cout << "[Info] Processing message queue with "
			<< _msgsQueue.size() << " messages...\n";
	// process every command in the message queue
	while (!_msgsQueue.empty())
	{
		Message& msg = _msgsQueue.front();
		numeric = _cmdReg.execute(*this, msg);
		if (numeric == irc::HASQUIT)
			_prepareClientDisconnect(msg.sender);
		else
		{
			msg.sender->setLastMsgTime(now);
			_msgsQueue.pop_front();
		}

	}
}

// Remove all queued messages sent by CLIENT.
void Server::_removeMsgsFrom(Client *client)
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
