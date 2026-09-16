/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_kick.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mweghofe <mweghofe@student.42vienna.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/04 06:33:39 by fpaglia           #+#    #+#             */
/*   Updated: 2026/09/16 12:22:47 by mweghofe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Message.hpp"
#include "Response.hpp"
#include "ft_irc.hpp"
#include "Command.hpp"
#include "IServerCtrl.hpp"
#include "Client.hpp"
#include "irc.hpp"

rfc cmd_kick(IServerCtrl & srv, const Message & msg)
{
	Client * sender = msg.sender;
	std::vector<std::string> users;
	Channel * channel = srv.getChannelByTitle(msg.params[0]);
	
	if (!channel)
		return irc::NOSUCHCHANNEL;
	if (!channel->isMember(sender))
		return irc::NOTONCHANNEL;
	if (!channel->isChanOp(sender))
		return irc::CHANOPRIVSNEEDED;
	users = irc::strSplit(msg.params[1], ',', false);

	for (size_t i = 0; i < users.size(); ++i)
	{
		Client *client2Kick = srv.findClientByNick(users[i]);
		if (!client2Kick || !channel->isMember(client2Kick))
		{
			srv.sendMessage(sender, 
							Response::buildNumeric(msg, irc::USERNOTINCHANNEL, users[i]));
			continue ;
		}
		// based on the info given here: https://defs.ircdocs.horse/defs/chanmembers
		// TODO: to be implemented after other PR are merged
		// if (channel->isFounder(client2Kick))
		// 	continue ;
		std::string trailing = "must have done something wrong.";
		if (msg.argCount() >= 2 && !(msg.flags & irc::MSG_HAS_TRAILING))
			srv.broadcast(channel, Response::buildRegular(msg, msg.params[0] + " " + users[i], trailing));
		else
			srv.broadcast(channel, Response::buildRegular(msg, msg.params[0] + " " + users[i]));
		srv.removeClientFromChannel(client2Kick, *channel, NULL);
	}

	return irc::OK;
}
