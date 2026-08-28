/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_mode.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mweghofe <mweghofe@student.42vienna.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/27 16:57:46 by fpaglia           #+#    #+#             */
/*   Updated: 2026/08/28 22:39:31 by mweghofe         ###   ########.fr       */
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

void printChannelModes(IServerCtrl& srv, Client* client,
					   const Message& msg, Channel* channel)
{
	(void) channel;
	srv.sendMessage(client,
			Response::buildNumeric(msg, irc::CHANNELMODEIS,
								   msg.params[0], "not handled yet"));
	// TODO query each bit and append to the output string if it exists
	// std::string reply;
	// TODO decide if we want to store timestamp or create; MODE can return it
}

/*

i: Set/remove Invite-only channel.
t: Set/remove restrictions on the TOPIC command.
k: Set/remove the channel key (password).			+   needs ARG
o: Give/take channel operator privilege.			+/- need ARG
l: Set/remove the user limit for the channel.		+   needs ARG

*/

rfc processModeRequests(IServerCtrl& srv, Client* client,
					    const Message& msg, Channel* channel)
{
	bool switcher = false;
	if (msg.params[1][0] != '+' &&  msg.params[1][0] != '-' )
		return (irc::UNKNOWNMODE);
	switcher = msg.params[1][0] == '+' ? true : false;
	std::string modes = msg.params[1].substr(1);
	if (modes.find_first_of('i') != modes.npos)
		channel->setInvite(switcher);
	/*
		some observations:
		- key must be removed before a new one can be set again
		- another +k just gets ignored (by inspircd)
		- removal of key prints old key
		- operators don't show in MODE #channel
		- limit can be replaced
		- output is +iklt key :limit (with inspircd)
		- multi removal is possible (non existing get ignored)
		- multi setting is possible, failure occurs individually
		- when sending +kl at the same time, the order doesn't matter
		  the values are taken as <key> <limit>, if limit is not a number -> 0
		  this is true for inspircd
	*/
	(void) srv; (void) client;
	return (irc::OK);
}

rfc handleChannelMode(IServerCtrl& srv, Client* client, const Message& msg)
{
	
	Channel*	channel = srv.getChannelByTitle(msg.params[0]);
	
	// 1) channel validation
	if (channel == NULL)
		return (irc::NOSUCHCHANNEL);
	// 2) mode query
	if (msg.params.size() == 1)
	{
		printChannelModes(srv, client, msg, channel);
		return (irc::OK);
	}
	// 3) OP validation
	if (!channel->isChanOp(client))
		return (irc::CHANOPRIVSNEEDED);
	// 4) check all params
	return (processModeRequests(srv, client, msg, channel));
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
	//		 irssi will register the correct nick on NICKNAMEINUSE
	else if (msg.params[0] == client->getNick())
		return (handleIrssiLogon(srv, client, msg));
	else
		return (irc::UMODEUNKNOWNFLAG);
}
