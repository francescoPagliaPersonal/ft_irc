/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server-addToChannel.cpp                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fpaglia <fpaglia@student.42vienna.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/21 13:14:43 by fpaglia           #+#    #+#             */
/*   Updated: 2026/08/26 14:20:29 by fpaglia          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"
#include "Channel.hpp"
#include "ft_irc.hpp"
#include <alloca.h>
#include <string>

// Add CLIENT to the channel CHNAME, creating it first if it does not
// exist yet; the channel password must match PW.
rfc Server::addToChannel(Client* client,
						  const std::string& title,
						  const std::string& pw = "")
{
	if (!(client->getChannelsList().size() < MAX_CHANJOIN))
		return irc::TOOMANYCHANNELS;
	if (!Channel::isTitleCompliant(title))
		return irc::BADCHANMASK;
	if (!irc::isNameCompliant(pw))
		return irc::BADCHANNELKEY;
	
	std::string mapKey = Channel::title2key(title);
	
	Channel* channel = _getChannel(mapKey);
	if (!channel)
	{
		channel = _addChannel(mapKey, title, pw);
		channel->addClient(client, US_FOUNDER | US_OPERATOR);
		client->addChannel(channel);
		return irc::OK;
	}
	if (channel->isMember(client))
		return irc::USERONCHANNEL;
	if (!channel->passwordMatch(pw))
		return irc::BADCHANNELKEY;
	if (!channel->belowChannelLimit())
		return irc::CHANNELISFULL;
	if (!channel->joinGranted(client))
		return irc::INVITEONLYCHAN;

	channel->addClient(client, US_BASIC);
	client->addChannel(channel);
		
	return irc::OK;
}
