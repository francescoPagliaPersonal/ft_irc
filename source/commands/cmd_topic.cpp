/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_topic.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fpaglia <fpaglia@student.42vienna.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/19 14:24:29 by fpaglia           #+#    #+#             */
/*   Updated: 2026/08/27 13:20:23 by mweghofe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// #include "ft_irc.hpp"
#include "Message.hpp"
#include "irc.hpp"
#include "Channel.hpp"
#include "Command.hpp"
#include "IServerCtrl.hpp"
#include "Response.hpp"

/*
std::string topicReply(const Message & msg, Channel * channel)
{
	if (channel->getTopic().empty())
		return Response::buildNumeric(msg, irc::NOTOPIC, channel->getTitle());
	std::string reply = Response::buildNumeric(msg, irc::TOPIC, 
									channel->getTitle(),
									channel->getTopic());
	return reply;

}

std::string addPropertyToNick(const Client & client, bitMask mask)
{
	std::string nick = client.getNick();
	
	if (mask & US_OPERATOR)
		return "@" + nick;
	return nick;
}

//  <client> <symbol> <channel> :[prefix]<nick>{ [prefix]<nick>}
std::string userListReply(const Message & msg, Channel* channel )
{
	std::map<Client*, bitMask> channelMembers = channel->getMembersMap();

	std::string response("");
	std::string tmp;
	std::string replyBase;
	
	replyBase = Response::buildNumeric(msg, irc::NAMREPLY, "= " + channel->getTitle() + " :");
	replyBase.erase(replyBase.size() - 2, 2); // remove CRLF

	std::map<Client*, bitMask>::const_iterator it = channelMembers.begin();
	for (; it != channelMembers.end(); ++it)
	{
		std::string nickToPrint = addPropertyToNick(*it->first, it->second);
		if (replyBase.size() + tmp.size() + nickToPrint.size() < MSG_MAX_LENGTH)
			tmp.append(nickToPrint + " ");				
		else 
		{
			response.append(replyBase + tmp + CRLF);
			tmp.clear();
			tmp.append(nickToPrint + " ");
		}
	}
	if (!tmp.empty())
		response.append(replyBase + tmp + CRLF);
	
	return response;			
}
*/

rfc cmd_topic(IServerCtrl & srv, const Message & msg)
{
	
	Client *client = msg.sender;
	Channel *channel = srv.getChannelByTitle(msg.params[0]);
	if (!channel)
		return irc::NOSUCHCHANNEL;
	if (!channel->isMember(client))
		return irc::NOTONCHANNEL;
	if (irc::argCount(msg) == 1)
	{
		if (channel->getTopic().empty())
			return irc::NOTOPIC;
		std::string reply;
		reply = irc::chunkifyTrailing(
							Response::buildNumeric(msg, irc::TOPIC, channel->getTitle()),
						channel->getTopic());
		srv.sendMessage(client, reply);
		return irc::OK;
	}
	if (irc::argCount(msg) == 2 
		&& channel->getModes() & CH_TOPIC
		&& !channel->isChanOp(client))
		return irc::CHANOPRIVSNEEDED;
	channel->setTopic(msg.trailing);
	std::string reply;
	reply = irc::chunkifyTrailing(
						Response::buildNumeric(msg, irc::TOPIC, channel->getTitle()),
					"");
	// TASKS:
	// send a join msg to the whole channel
	srv.broadcast(channel, reply);

	return irc::OK;
}
