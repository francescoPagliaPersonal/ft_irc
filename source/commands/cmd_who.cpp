/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_who.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mweghofe <mweghofe@student.42vienna.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/15 17:00:00 by mweghofe          #+#    #+#             */
/*   Updated: 2026/09/15 17:00:00 by mweghofe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Channel.hpp"
#include "Client.hpp"
#include "Command.hpp"
#include "IServerCtrl.hpp"
#include "Message.hpp"
#include "Response.hpp"
#include "irc.hpp"

#include <map>

// Minimal WHO: answer WHO <channel> with one 352 per member plus 315.
// Flags are always H (present, we track no away state) plus @ for chanops.
rfc cmd_who(IServerCtrl& srv, const Message& msg)
{
	Client* client = msg.sender;

	if (!(msg.flags & irc::MSG_HAS_PARAMS))
		return (irc::NEEDMOREPARAMS);
	Channel* channel = srv.getChannelByTitle(msg.params[0]);
	if (channel == NULL)
		return (irc::NOSUCHCHANNEL);
	std::map<Client*, bitMask> members = channel->getMembersMap();
	std::map<Client*, bitMask>::const_iterator it;
	for (it = members.begin(); it != members.end(); ++it)
	{
		Client* member = it->first;
		std::string flags("H");
		if (it->second & US_OPERATOR)
			flags.append("@");
		std::string args = channel->getTitle() + " "
			+ member->getUserName() + " "
			+ member->getHost() + " "
			+ Response::getServerName() + " "
			+ member->getNick() + " "
			+ flags;
		srv.sendMessage(client,
			Response::buildNumeric(msg, irc::WHOREPLY, args,
				"0 " + member->getRealName()));
	}
	srv.sendMessage(client,
		Response::buildNumeric(msg, irc::ENDOFWHO, channel->getTitle()));
	return (irc::OK);
}
