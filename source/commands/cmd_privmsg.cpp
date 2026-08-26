/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_privmsg.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fpaglia <fpaglia@student.42vienna.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/19 15:31:28 by mweghofe          #+#    #+#             */
/*   Updated: 2026/08/26 14:28:44 by fpaglia          ###   ########.fr       */
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
	// TODO do we want proper return codes ie. 401, 411,412 ERR_NOTEXTTOSEND
	// TODO argument policy currently prevents a missing recipient
	// TODO neither return has a message yet
	if (!(msg.flags & MSG_HAS_PARAMS))
		return (irc::NORECIPIENT);
	if (!(msg.flags & MSG_HAS_TRAILING))
		return (irc::NOTEXTTOSEND);
	std::vector<std::string>	recipients = irc::strSplit(msg.params[0], ',', false);
	for (size_t i = 0; i < recipients.size(); ++i)
	{
		if (recipients[i][0] != '#' && recipients[i][0] != '&' )
		{
			Client* recipient = srv.findClientByNick(recipients[i]);
			if (recipient == NULL)
				srv.sendMessage(*sender, Response::noOpt(msg, irc::NOSUCHNICK));
			else
			{
				std::string reply = Response::senderMessage(msg, recipients[i]);
				srv.sendMessage(*recipient, reply);
			}
			continue;
		}
		Channel* channel = srv.getChannelByTitle(recipients[i]);
		if (channel == NULL)
			srv.sendMessage(*sender, Response::noOpt(msg, irc::NOSUCHCHANNEL));
		else
		{
			std::string reply = Response::senderMessage(msg, recipients[i]);
			srv.broadcast(reply, channel, sender);
		}

	}
	
	return (irc::OK);
}
