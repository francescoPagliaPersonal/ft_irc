/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_topic.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fpaglia <fpaglia@student.42vienna.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/19 14:24:29 by fpaglia           #+#    #+#             */
/*   Updated: 2026/09/15 17:50:48 by fpaglia          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Message.hpp"
#include "irc.hpp"
#include "Channel.hpp"
#include "Command.hpp"
#include "IServerCtrl.hpp"
#include "Response.hpp"

#include <sstream>
#include <iostream>

namespace {

	std::string _buildPrefix(const Message& msg, irc::rfc code, const std::string & channelTitle)
	{
		std::stringstream prefix;
		prefix
			<< ":" << Response::getServerName() << " " 
			<< code << " "
			<< msg.sender->getNick() << " "
			<< channelTitle << " :";		
		return (prefix.str());
	}
}


rfc cmd_topic(IServerCtrl & srv, const Message & msg)
{
	
	Client *client = msg.sender;
	Channel *channel = srv.getChannelByTitle(msg.params[0]);
	
	if (!channel)
		return irc::NOSUCHCHANNEL;
	if (!channel->isMember(client))
		return irc::NOTONCHANNEL;
	
	if (irc::argCount(msg) == 1 && !(msg.flags & irc::MSG_HAS_TRAILING))
	{
		if (channel->getTopic().empty())
			return irc::NOTOPIC;
	}
	else if (irc::argCount(msg) == 2)
	{
		if (channel->getModes() & CH_TOPIC && !channel->isChanOp(client))
			return irc::CHANOPRIVSNEEDED;
		if (msg.trailing.empty())
			channel->setTopic("");
		else
			channel->setTopic(msg.trailing);
	}
	
	std::string reply;
	reply = irc::chunkifyTrailing(
						_buildPrefix(msg, irc::TOPIC, channel->getTitle() ),
					channel->getTopic());
					
	srv.broadcast(channel, reply);

	return irc::OK;
}
