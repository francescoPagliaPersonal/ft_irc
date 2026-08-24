/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server-Broadcast.cpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mweghofe <mweghofe@student.42vienna.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/19 17:36:18 by fpaglia           #+#    #+#             */
/*   Updated: 2026/08/24 15:00:07 by mweghofe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"
#include "Channel.hpp"
#include "ft_irc.hpp"

// Send MSG to all members of CHANNEL, optionally excluding SENDER.
void Server::broadcast(const std::string& reply, Channel* channel, Client* client)
{
	std::map<Client*, t_uint8> channelMembers = channel->getMembersMap();
	std::map<Client*, t_uint8>::const_iterator it = channelMembers.begin();
	for (; it != channelMembers.end(); ++it)
	{
		if (it->first != client)
			sendMessage(*it->first, reply);
	}
}
