/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_privmsg.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mweghofe <mweghofe@student.42vienna.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/19 15:31:28 by mweghofe          #+#    #+#             */
/*   Updated: 2026/09/18 13:01:49 by mweghofe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Channel.hpp"
#include "Command.hpp"
#include "IServerCtrl.hpp"
#include "Client.hpp"
#include "Message.hpp"
#include "irc.hpp"
#include <sstream>
#include <vector>
#include "Response.hpp"

rfc cmd_privmsg(IServerCtrl& srv, const Message& msg)
{
	Client* sender = msg.sender;
	if (!(msg.flags & irc::MSG_HAS_PARAMS))
		return (irc::NORECIPIENT);
	if (msg.params.size() == 1 && msg.flags & irc::MSG_HAS_TRAILING)
		return (irc::NORECIPIENT);
	if (!(msg.flags & irc::MSG_HAS_TRAILING))
		return (irc::NOTEXTTOSEND);
	std::vector<std::string>	recipients = irc::strSplit(msg.params[0], ',', false);
	for (size_t i = 0; i < recipients.size(); ++i)
	{
		if (recipients[i][0] != '#' && recipients[i][0] != '&' )
		{
			Client* recipient = srv.findClientByNick(recipients[i]);
			if (recipient == NULL)
				srv.sendMessage(sender, Response::buildNumeric(msg, irc::NOSUCHNICK));
			else
			{
				std::string reply = Response::buildRegular(msg, recipients[i]);
				srv.sendMessage(recipient, reply);
			}
			continue;
		}
		Channel* channel = srv.getChannelByTitle(recipients[i]);
		if (channel == NULL)
			srv.sendMessage(sender, Response::buildNumeric(msg, irc::NOSUCHCHANNEL));
		else
		{
			if (!channel->isMember(sender))
				return irc::NOTONCHANNEL;
			std::string reply = Response::buildRegular(msg, recipients[i]);
			srv.broadcast(channel, sender, reply);
		}
	}
	
	return (irc::OK);
}
