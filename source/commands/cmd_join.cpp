/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_join.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fpaglia <fpaglia@student.42vienna.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/19 14:24:29 by fpaglia           #+#    #+#             */
/*   Updated: 2026/08/21 17:27:06 by fpaglia          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Channel.hpp"
# include "Command.hpp"
# include "IServerCtrl.hpp"
# include "Client.hpp"
#include "ft_irc.hpp"
#include <cctype>
#include <sstream>
#include <string>
#include <vector>


std::string topicReply(Client * client, Channel * channel)
{
	std::stringstream reply;
	reply << ":CoolServ" << " ";
	
	if (!channel->getTopic().empty())
	{
		reply 
			<< "331" << " "
			<< client->getNick() << " "
			<< channel->getTitle() << " :No topic is set."
			<< CRLF;
		return reply.str();
	}
	reply 
		<< "332" << " "
		<< client->getNick() << " "
		<< channel->getTitle() << " :"
		<< channel->getTopic()
		<< CRLF;
	return reply.str();
}

std::string addPropertyToNick(const Client & client, bitMask mask)
{
	std::string nick = client.getNick();
	
	if (mask & US_OPERATOR)
		return "@" + nick;
	return nick;
}

//  <client> <symbol> <channel> :[prefix]<nick>{ [prefix]<nick>}
std::string userListReply(Client *client, Channel* channel )
{
	std::map<Client*, bitMask> channelMembers = channel->getMembersMap();
	std::stringstream reply;
	std::string response("");
	
	reply
		<< ":CoolServ" << " " << "353" << " "
		<< client->getNick() << " "
		<< "=" << " " // sets the status of the channel to public (@ secret, * private)
		<< channel->getTitle() << " "
		<< ":";
		
	std::string replyBase = reply.str();
	std::string tmp;

	std::map<Client*, bitMask>::const_iterator it = channelMembers.begin();
	for (; it != channelMembers.end(); ++it)
	{
		std::string nickToPrint = addPropertyToNick(*it->first, it->second);  // TODO: add Property to nick  
		if (replyBase.size() + tmp.size() + nickToPrint.size() < MSG_MAX_LENGTH)
			tmp.append(nickToPrint + " ");				
		else 
		{
			response.append(replyBase + tmp + CRLF);
			tmp.clear();
		}
	}
	if (!tmp.empty())
		response.append(replyBase + tmp + CRLF);
	
	return response;			
}
std::string endOfNames(const std::string &client, const std::string &channel)
{
	std::stringstream response;
	response
		<< ":CoolServ" << " " << "366" << " "
		<< client << " "
		<< channel << " "
		<< ":End of /NAMES list" << CRLF;
		
	return response.str();
}

int cmd_join(IServerCtrl & srv, const Message & msg)
{
	std::vector<std::string>	channels;
	std::vector<std::string>	passwords;
	
	Client *client = msg.sender;
	channels = strSplit(msg.params[0], ',', false);
	if (msg.params.size() == 2)
		passwords = strSplit(msg.params[1], ',', true);
	else
	{
		
		for (size_t i = passwords.size(); i < channels.size(); ++i)
			passwords.push_back("");		
	}

	std::stringstream reply;
	
	for (size_t i = 0; i < channels.size(); ++i)
	{
		
		int ret = srv.addToChannel(client, channels[i], passwords[i]);
		if (ret)
		{
			reply 
				<< ":CoolServ" << " " << ret << " " 
				<< client->getNick() << " " << channels[i] << " "
				<< ":An issue with the channel has occurred" 
				<< CRLF;
			srv.sendMessage(*client, reply.str());
			continue;
		}
		reply	
			<< ":" << client->getNick() << " "
			<< msg.command << " "
			<< channels[i] << " " 
			<< CRLF;

		Channel *channel = srv.getChannelByTitle(channels[i]);
		
		// TASKS:
		// send a join msg to the whole channel

		std::map<Client *, bitMask>	channelMembers = channel->getMembersMap();
		srv.broadcast(reply.str(), channelMembers, client);

		// send the topic of the channel to the client
					
		srv.sendMessage(*client, topicReply(client, channel));
		
		// send the list of users to the client			
		srv.sendMessage(*client, userListReply(client, channel));
		srv.sendMessage(*client, endOfNames(client->getNick(), channel->getTitle()));
	
	}
	
	return rfc::OK;
}
