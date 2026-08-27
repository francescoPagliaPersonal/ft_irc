/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server-Broadcast.cpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mweghofe <mweghofe@student.42vienna.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/19 17:36:18 by fpaglia           #+#    #+#             */
/*   Updated: 2026/08/27 13:19:49 by mweghofe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"
#include "Channel.hpp"
#include "ft_irc.hpp"

// Send MSG to all members of CHANNEL, excluding SENDER.
void Server::broadcast(Channel* channel, Client* client, const std::string& reply)
{
	std::map<Client*, irc::uint8> channelMembers = channel->getMembersMap();
	std::map<Client*, irc::uint8>::const_iterator it = channelMembers.begin();
	for (; it != channelMembers.end(); ++it)
	{
		if (it->first != client)
			sendMessage(it->first, reply);
	}
}

// Send MSG to all members of CHANNEL.
void Server::broadcast(Channel* channel, const std::string& reply)
{
	std::map<Client*, irc::uint8> channelMembers = channel->getMembersMap();
	std::map<Client*, irc::uint8>::const_iterator it = channelMembers.begin();
	for (; it != channelMembers.end(); ++it)
	{
		sendMessage(it->first, reply);
	}
}
