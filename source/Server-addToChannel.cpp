/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server-addToChannel.cpp                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fpaglia <fpaglia@student.42vienna.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/21 13:14:43 by fpaglia           #+#    #+#             */
/*   Updated: 2026/08/21 13:35:15 by fpaglia          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"
#include "Channel.hpp"
#include "ft_irc.hpp"
#include <string>

// Add CLIENT to the channel CHNAME, creating it first if it does not
// exist yet; the channel password must match PW.
int Server::addToChannel(Client* client,
						  const std::string& title,
						  const std::string& pw = "")
{
	Channel* channel = _getChannel(title);
	if (!channel)
	{
		channel =_newChannel(title, pw);
		channel->addClient(client, US_FOUNDER | US_OPERATOR);
		client->addChannel(channel);
	}
	// TODO does the client or the server check IF client is a member?
	// if (client->isChannelMember(channel))
	// it is more meaningfull that the channel checks the membership 
	// the client information is a convenince for us 
	if (channel->isMember(client))
	{
		std::cout << "[Info] Client is already member of that channel - consequence not handled yet.\n";
		return rfc::USERONCHANNEL;
	}
	if (!channel->passwordMatch(pw))
		return rfc::BADCHANNELKEY;
	if (!channel->hasRights(client))
	{
		// if (channel->getModes() & CH_INVITE)
		channel->addClient(client, US_BASIC);
		client->addChannel(channel);
		broadcastToChannel(channel, "[Info] New Client joined channel - needs proper msg.", client);
	}
	else // TODO needs some proper code or just return false and cmd must send different reply
	{
		std::cout << "[Info] Channel password incorrect - consequence not handled yet.\n";
	}
	return rfc::OK;
}