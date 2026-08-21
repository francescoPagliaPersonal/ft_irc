/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_join.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fpaglia <fpaglia@student.42vienna.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/19 14:24:29 by fpaglia           #+#    #+#             */
/*   Updated: 2026/08/21 12:30:49 by fpaglia          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Channel.hpp"
# include "Command.hpp"
# include "IServerCtrl.hpp"
# include "Client.hpp"
#include "ft_irc.hpp"
#include <algorithm>
#include <cctype>
#include <sstream>
#include <string>
#include <vector>


void sendTopic(Channel *channel)
{
	(void) channel;
}

std::string addPropertyToNick(const Client & client, bitMask mask)
{
	(void)mask;
	return client.getNick();
}

namespace {
	
}
//TODO: if a x is found and the pwd is empty then grant connection.

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
		if (channels[i][0] == '#' || channels[i][0] == '&')
		{
			if (!Channel::isTitleCompliant(channels[i].substr(0)))
				return rfc::BADCHANMASK;
			if (!Client::isNickCompliant(passwords[i]))
				return rfc::BADCHANNELKEY;
			// TODO revisit the return types of the add to Channels
			int ret = srv.addToChannel(client, channels[i].substr(1), passwords[i]);
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

			Channel *channel = srv.getChannelByTitle(channels[i].substr(1));
			
			// TASKS:
			// send a join msg to the whole channel

			std::map<Client *, bitMask>	clientMap = channel->getClientMap();
			srv.broadcast(reply.str(), clientMap, client);

			// send the topic of the channel to the client
			if (!channel->getTopic().empty())
				sendTopic(channel);
			
			// send the list of users to the client			
			//  <client> <symbol> <channel> :[prefix]<nick>{ [prefix]<nick>}
			reply.clear();
			reply
				<< ":CoolServ" << " " << "353" << " "
				<< client->getNick() << " "
				<< "=" << " " // sets the status of the channel to public (@ secret, * private)
				<< channels[i] << " "
				<< ":";
			std::string replyBase = reply.str();
			std::string tmp;

			std::map<Client*, bitMask>::const_iterator it = clientMap.begin();
			for (; it != clientMap.end(); ++it)
			{
				std::string nickToPrint = addPropertyToNick(*it->first, it->second);  // TODO: add Property to nick  
				if (replyBase.size() + tmp.size() + nickToPrint.size() < MSG_MAX_LENGTH)
					tmp.append(nickToPrint + " ");				
				else 
				{
					srv.sendMessage(*client, replyBase + tmp + CRLF);
					tmp.clear();
				}
			}
			if (!tmp.empty())
				srv.sendMessage(*client, replyBase + tmp + CRLF);
			
			std::stringstream reply1;
			reply1
				<< ":CoolServ" << " " << "366" << " "
				<< client->getNick() << " "
				<< channels[i] << " "
				<< ":End of /NAMES list" << CRLF;
			srv.sendMessage(*client, reply1.str());
		}
		
		else
		{
			reply 
				<< ":CoolServ" << " " << rfc::BADCHANMASK << " " 
				<< client->getNick() << " " << channels[i] << " "
				<< ":Bad Channel Mask" 
				<< CRLF;
			srv.sendMessage(*client, reply.str());
		}
	}
	
	return rfc::OK;
}
