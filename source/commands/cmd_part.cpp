/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_part.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mweghofe <mweghofe@student.42vienna.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/07 16:34:48 by fpaglia           #+#    #+#             */
/*   Updated: 2026/09/07 23:14:40 by mweghofe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Channel.hpp"
#include "Command.hpp"
#include "Message.hpp"
#include "Response.hpp"
#include "irc.hpp"
#include "IServerCtrl.hpp"


rfc cmd_part(IServerCtrl & srv, const Message & msg)
{
	Client *client = msg.sender;
	std::vector<std::string>	channels;
	std::string reply;
	std::set<Client *> Members;

	channels = irc::strSplit(msg.params[0], ',', false);

	for (size_t i = 0; i < channels.size(); ++i)
	{
		if (channels[i][0] != '#' && channels[i][0] != '&' )
		{
			reply = Response::buildNumeric(msg, irc::NOSUCHCHANNEL, channels[i]);
			srv.sendMessage(client, reply);
			continue ;
		}
		Channel *channel = srv.getChannelByTitle(channels[i]);
		if (!channel)
		{
			reply = Response::buildNumeric(msg, irc::NOSUCHCHANNEL, channels[i]);
			srv.sendMessage(client, reply);
			continue ;
		}
		if (!client->isChannelMember(channel))
		{
			reply = Response::buildNumeric(msg, irc::NOTONCHANNEL, channels[i]);
			srv.sendMessage(client, reply);
			continue;
		}
		if (msg.params.size() == 2)
			reply = Response::buildRegular(msg, channels[i], msg.params[1]);
		else
			reply = Response::buildRegular(msg, channels[i]);
		
		srv.broadcast(channel, reply);
		srv.removeClientFromChannel(client, *channel, &Members);
		client->removeChannel(channel);
	}
	return irc::OK;
}
