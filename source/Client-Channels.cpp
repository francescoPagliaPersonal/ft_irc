/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client-Channels.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mweghofe <mweghofe@student.42vienna.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/17 17:10:07 by mweghofe          #+#    #+#             */
/*   Updated: 2026/08/17 17:10:39 by mweghofe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Client.hpp"

// -------------------------------------------------------------------------- //
// CHANNEL INTERACTION
// -------------------------------------------------------------------------- //

void Client::addChannel(Channel* channel)
{
	_channels.push_back(channel);
}

void Client::removeChannel(Channel* channel)
{
	std::vector<Channel*>::iterator it;
	for (it = _channels.begin(); it != _channels.end(); it++)
	{
		if (*it == channel)
		{
			_channels.erase(it);
			break ;
		}
	}
}

bool Client::isChannelMember(Channel* channel) const
{
	std::vector<Channel*>::const_iterator it;
	for (it = _channels.begin(); it != _channels.end(); it++)
	{
		if (*it == channel)
		{
			return (true);
		}
	}
	return (false);
}
