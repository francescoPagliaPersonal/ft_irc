/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_mode.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mweghofe <mweghofe@student.42vienna.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/27 16:57:46 by fpaglia           #+#    #+#             */
/*   Updated: 2026/08/28 11:44:20 by mweghofe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Channel.hpp"
#include "Client.hpp"
#include "Command.hpp"
#include "IServerCtrl.hpp"
#include "Message.hpp"
#include "irc.hpp"
#include "Response.hpp"

namespace
{

rfc handleChannelMode(IServerCtrl& srv, Client* client, const Message& msg)
{
	if (msg.params.size() == 1)
	{
		srv.sendMessage(client,
			Response::buildNumeric(msg, irc::CHANNELMODEIS, msg.params[0], "not handled yet"));
	}

	Channel*	channel = srv.getChannelByTitle(msg.params[0]);
	bool		switcher = false;

	if (channel == NULL)
		return (irc::NOSUCHCHANNEL);
	if (!channel->isChanOp(client))
		return (irc::CHANOPRIVSNEEDED);
	if (msg.params[1][0] != '+' &&  msg.params[1][0] != '-' )
		return (irc::UNKNOWNMODE);
	switcher = msg.params[1][0] == '+' ? true : false;
	std::string modes = msg.params[1].substr(1);
	if (modes.find_first_of('i') != modes.npos)
		channel->setInvite(switcher);
	return (irc::OK);
}

rfc handleIrssiLogon(IServerCtrl&srv, Client* client, const Message& msg)
{
	if (msg.params.size() == 2 && msg.params[1] == "+i")
	{
		// either of those stubs fixes the irssi NICKNAMEINUSE interface update issue
		// error message does NOT
		srv.sendMessage(client,
			Response::buildNumeric(msg, irc::UMODEIS, "", "not supported"));
			// Response::buildNumeric(msg, irc::UMODEIS, "+i"));
			// Response::buildRegular(msg, client->getNick(), msg.params[1]));
		return (irc::OK);
	}
	else
		return (irc::UMODEUNKNOWNFLAG);
}

} // end of namespace

rfc cmd_mode(IServerCtrl& srv, const Message& msg)
{
	Client *client = msg.sender;
	// TODO this assumes, trailing is copied into param
	// we CANNOT get here w/o either params or trailing used! correct?
	if (msg.params[0][0] == '#' || msg.params[0][0] == '&')
		return (handleChannelMode(srv, client, msg));
	// TODO fine like that? if we have this as a DUMMY,
	//		 irssi will register the correct nick on NICKCOLLISION
	else if (msg.params[0] == client->getNick())
		return (handleIrssiLogon(srv, client, msg));
	else
		return (irc::UMODEUNKNOWNFLAG);
}
