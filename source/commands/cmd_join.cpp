/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_join.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mweghofe <mweghofe@student.42vienna.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/19 14:24:29 by fpaglia           #+#    #+#             */
/*   Updated: 2026/08/24 18:31:54 by mweghofe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Channel.hpp"
# include "Command.hpp"
# include "IServerCtrl.hpp"
# include "Client.hpp"
# include "irc.hpp"
#include <cctype>
#include <sstream>
#include <string>
#include <vector>

std::string topicReply(Client * client, Channel * channel)
{
	std::stringstream reply;
	reply << ":CoolServ" << " ";
	
	if (channel->getTopic().empty())
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
		// FIXME nick reply needs 'nicklist_set_host'. the test below fixed the error. still needs proper address handled
		// nickToPrint += "!localhost";
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
		srv.broadcast(reply.str(), channel, client);

		// send the topic of the channel to the client
					
		srv.sendMessage(*client, topicReply(client, channel));
		
		// send the list of users to the client			
		srv.sendMessage(*client, userListReply(client, channel));
		srv.sendMessage(*client, endOfNames(client->getNick(), channel->getTitle()));
	
		reply.clear();
		
	}
	
	return irc::OK;
}
