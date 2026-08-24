/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server-addToChannel.cpp                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mweghofe <mweghofe@student.42vienna.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/21 13:14:43 by fpaglia           #+#    #+#             */
/*   Updated: 2026/08/24 14:30:02 by mweghofe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"
#include "Channel.hpp"
#include "ft_irc.hpp"
#include <alloca.h>
#include <string>

// Add CLIENT to the channel CHNAME, creating it first if it does not
// exist yet; the channel password must match PW.
int Server::addToChannel(Client* client,
						  const std::string& title,
						  const std::string& pw = "")
{
	if (!(client->getChannelsList().size() < MAX_CHANJOIN))
		return rfc::TOOMANYCHANS;
	if (!Channel::isTitleCompliant(title))
		return rfc::BADCHANMASK;
	if (!Client::isNickCompliant(pw))
		return rfc::BADCHANNELKEY;
	
	std::string mapKey = Channel::title2key(title);
	
	Channel* channel = _getChannel(mapKey);
	if (!channel)
	{
		channel = _addChannel(mapKey, title, pw);
		channel->addClient(client, US_FOUNDER | US_OPERATOR);
		client->addChannel(channel);
		return rfc::OK;
	}
	if (channel->isMember(client))
		return rfc::USERONCHANNEL;
	if (!channel->passwordMatch(pw))
		return rfc::BADCHANNELKEY;
	if (!channel->belowChannelLimit())
		return rfc::CHANFULL;
	if (!channel->joinGranted(client))
		return rfc::INVITEONLY;

	channel->addClient(client, US_BASIC);
	client->addChannel(channel);
		
	return rfc::OK;
}
