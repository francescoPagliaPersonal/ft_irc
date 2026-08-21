/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server-addToChannel.cpp                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fpaglia <fpaglia@student.42vienna.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/21 13:14:43 by fpaglia           #+#    #+#             */
/*   Updated: 2026/08/21 15:48:59 by fpaglia          ###   ########.fr       */
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
	// TODO does the client or the server check IF client is a member?
	// if (client->isChannelMember(channel))
	// it is more meaningfull that the channel checks the membership 
	// the client information is a convenince for us 
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