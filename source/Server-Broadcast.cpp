/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server-Broadcast.cpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mweghofe <mweghofe@student.42vienna.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/19 17:36:18 by fpaglia           #+#    #+#             */
/*   Updated: 2026/09/01 09:27:17 by mweghofe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"
#include "Channel.hpp"
#include "ft_irc.hpp"

// Send message REPLY to all members of CHANNEL, excluding the sender CLIENT.
void Server::broadcast(Channel* channel, Client* client, const std::string& reply) const
{
	std::map<Client*, irc::uint8> channelMembers = channel->getMembersMap();
	std::map<Client*, irc::uint8>::const_iterator it = channelMembers.begin();
	for (; it != channelMembers.end(); ++it)
	{
		if (it->first != client)
			sendMessage(it->first, reply);
	}
}

// Send message REPLY to all members of CHANNEL.
void Server::broadcast(Channel* channel, const std::string& reply) const
{
	std::map<Client*, irc::uint8> channelMembers = channel->getMembersMap();
	std::map<Client*, irc::uint8>::const_iterator it = channelMembers.begin();
	for (; it != channelMembers.end(); ++it)
	{
		sendMessage(it->first, reply);
	}
}

//Send message REPLY to all clients in RECIPIENTS.
void Server::broadcast(std::set<Client*>& recipients, const std::string& reply) const
{
	std::set<Client*>::const_iterator it;
	for (it = recipients.begin(); it != recipients.end(); it++)
	{
		sendMessage(*it, reply);
	}
}
