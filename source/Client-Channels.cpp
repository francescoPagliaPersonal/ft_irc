/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client-Channels.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mweghofe <mweghofe@student.42vienna.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/17 17:10:07 by mweghofe          #+#    #+#             */
/*   Updated: 2026/08/27 23:21:35 by mweghofe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Client.hpp"
#include "Channel.hpp"

// -------------------------------------------------------------------------- //
// CHANNEL INTERACTION
// -------------------------------------------------------------------------- //

// Add CHANNEL to the client's list of joined channels.
void Client::addChannel(Channel* channel)
{
	_channels.push_back(channel);
}

// Remove CHANNEL from the client's list of joined channels.
void Client::removeChannel(Channel* channel)
{
	std::deque<Channel*>::iterator it;
	for (it = _channels.begin(); it != _channels.end(); it++)
	{
		if (*it == channel)
		{
			_channels.erase(it);
			break ;
		}
	}
}

// Removes the client from all channels and clears the membership list.
void Client::removeFromChannels()
{
	std::deque<Channel*>::iterator it;
	for (it = _channels.begin(); it != _channels.end(); it++)
	{
		(*it)->removeClient(this);
	}
	_channels.clear();
}

// Check if the client is a member of CHANNEL.
bool Client::isChannelMember(Channel* channel) const
{
	std::deque<Channel*>::const_iterator it;
	for (it = _channels.begin(); it != _channels.end(); it++)
	{
		if (*it == channel)
		{
			return (true);
		}
	}
	return (false);
}
