/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_pong.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mweghofe <mweghofe@student.42vienna.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/10 14:07:51 by mweghofe          #+#    #+#             */
/*   Updated: 2026/09/10 18:09:19 by mweghofe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_irc.hpp"
#include "Command.hpp"
#include "irc.hpp"
#include "Client.hpp"
#include "IServerCtrl.hpp"
#include "Response.hpp"

/*
		409    ERR_NOORIGIN
              ":No origin specified"

         - PING or PONG message missing the originator parameter.


repurpose Client::_lastSpamTime to Client::_lastMsgTime
use it for both spam detection and ping/pong timing
Command::execute(IServerCtrl& srv, const Message& msg) can do the regular update
Server::_housekeeping() compares now with _lastMsgTime before sending a PING

*/

rfc cmd_pong(IServerCtrl& srv, const Message& msg)
{
	(void) srv;
	Client* client = msg.sender;
	if (!(msg.flags & irc::MSG_HAS_PARAMS) && !(msg.flags & irc::MSG_HAS_TRAILING))
		return (irc::NOORIGIN);
	if (msg.flags & irc::MSG_HAS_PARAMS)
	{
		if (msg.params[0] != client->getNick())
			return (irc::NOSUCHNICK);
	} 
	else if (msg.flags & irc::MSG_HAS_TRAILING)
	{
		if (msg.trailing != client->getNick())
		{ // HACK this triggers w/o params populated; i think we still have other param[0] dereferences when there could be no param
			srv.sendMessage(client, Response::buildNumeric(msg, irc::NOSUCHNICK));
			return (irc::OK);
		}
	}
	// internal timer is updated by Server::_executeCommands
	std::cout << "[FD " << client->getFD() << "] Resetting ping counter...\n";
	client->resetPingCount();
	return (irc::OK);
}
