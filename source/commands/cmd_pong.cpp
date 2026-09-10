/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_pong.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mweghofe <mweghofe@student.42vienna.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/10 14:07:51 by mweghofe          #+#    #+#             */
/*   Updated: 2026/09/10 14:32:06 by mweghofe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_irc.hpp"
#include "Command.hpp"
#include "irc.hpp"

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
	if (!(msg.flags & irc::MSG_HAS_PARAMS) && !(msg.flags & irc::MSG_HAS_TRAILING))
		return (irc::NOORIGIN);
	// TODO shall we just ignore multiple provided sesrvers?
	if ((msg.flags & irc::MSG_HAS_PARAMS) && msg.params[0] != "CoolServ") // FIXME server name macro
		return (irc::NOSUCHSERVER);
	if ((msg.flags & irc::MSG_HAS_TRAILING) && msg.trailing != "CoolServ") // FIXME server name macro
		return (irc::NOSUCHSERVER);
	// internal timer is updated by Server::_executeCommands
	return (irc::OK);
}
