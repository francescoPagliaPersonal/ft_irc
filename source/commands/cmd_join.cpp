/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_join.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mweghofe <mweghofe@student.42vienna.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/19 14:24:29 by fpaglia           #+#    #+#             */
/*   Updated: 2026/08/26 16:04:19 by mweghofe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Channel.hpp"
# include "Command.hpp"
# include "IServerCtrl.hpp"
# include "Client.hpp"
#include "Message.hpp"
# include "irc.hpp"
#include <cctype>
#include <sstream>
#include <string>
#include <vector>
#include "Response.hpp"

std::string topicReply(const Message & msg, Channel * channel)
{
	if (channel->getTopic().empty())
		return Response::args(msg, irc::NOTOPIC, channel->getTitle());
	std::string reply = Response::trailing(msg, irc::TOPIC, 
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
	
	replyBase = Response::args(msg, irc::NAMREPLY, "= " + channel->getTitle() + " :");

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

rfc cmd_join(IServerCtrl & srv, const Message & msg)
{
	std::vector<std::string>	channels;
	std::vector<std::string>	passwords;
	
	Client *client = msg.sender;
	channels = irc::strSplit(msg.params[0], ',', false);
	if (msg.params.size() == 2)
		passwords = irc::strSplit(msg.params[1], ',', true);

	for (size_t i = passwords.size(); i < channels.size(); ++i)
		passwords.push_back("");		

	for (size_t i = 0; i < channels.size(); ++i)
	{

		rfc numeric = srv.addToChannel(client, channels[i], passwords[i]);
		if (numeric)
		{
			srv.sendMessage(*client, Response::args(msg, numeric, channels[i]));
			continue;
		}
		
		std::string reply = Response::senderMessage(msg, channels[i]);
		
		Channel *channel = srv.getChannelByTitle(channels[i]);
		
		// TASKS:
		// send a join msg to the whole channel
		srv.broadcast(reply, channel);

		// send the topic of the channel to the client
					
		srv.sendMessage(*client, topicReply(msg, channel));
		
		// send the list of users to the client			
		srv.sendMessage(*client, userListReply(msg, channel));
		srv.sendMessage(*client, Response::args(msg, irc::ENDOFNAMES, channel->getTitle()));
	}
	return irc::OK;
}
