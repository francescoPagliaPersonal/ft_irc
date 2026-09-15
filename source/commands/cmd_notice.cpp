/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_notice.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mweghofe <mweghofe@student.42vienna.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/15 17:10:00 by mweghofe          #+#    #+#             */
/*   Updated: 2026/09/15 17:10:00 by mweghofe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Channel.hpp"
#include "Client.hpp"
#include "Command.hpp"
#include "IServerCtrl.hpp"
#include "Message.hpp"
#include "Response.hpp"
#include "irc.hpp"

#include <vector>

// NOTICE routes like PRIVMSG but never sends routing errors back,
// so automatic replies can never loop between clients.
rfc cmd_notice(IServerCtrl& srv, const Message& msg)
{
	Client* sender = msg.sender;

	if (!(msg.flags & irc::MSG_HAS_PARAMS))
		return (irc::NORECIPIENT);
	if (!(msg.flags & irc::MSG_HAS_TRAILING))
		return (irc::NOTEXTTOSEND);
	std::vector<std::string> recipients = irc::strSplit(msg.params[0], ',', false);
	for (size_t i = 0; i < recipients.size(); ++i)
	{
		if (recipients[i][0] != '#' && recipients[i][0] != '&')
		{
			Client* recipient = srv.findClientByNick(recipients[i]);
			if (recipient == NULL)
				continue ;
			srv.sendMessage(recipient,
				Response::buildRegular(msg, recipients[i]));
			continue ;
		}
		Channel* channel = srv.getChannelByTitle(recipients[i]);
		if (channel == NULL)
			continue ;
		if (!channel->isMember(sender))
			continue ;
		srv.broadcast(channel, sender,
			Response::buildRegular(msg, recipients[i]));
	}
	return (irc::OK);
}
