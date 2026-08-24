/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server-Channels.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mweghofe <mweghofe@student.42vienna.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/17 13:55:04 by mweghofe          #+#    #+#             */
/*   Updated: 2026/08/24 14:59:15 by mweghofe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_irc.hpp"
#include "Server.hpp"
#include "Channel.hpp"
#include <cstddef>

// -------------------------------------------------------------------------- //
// INTERFACE -- CHANNELS
// -------------------------------------------------------------------------- //


Channel* Server::getChannelByTitle(std::string title)
{
	return _getChannel(Channel::title2key(title));
}

// Remove CLIENT from the channel CHNAME and clean up empty channels.
void Server::removeFromChannel(Client* client, const std::string& title,
							   const std::string& reason)
{
	// TODO does the client or the server check IF client is a member?
	Channel* channel = _getChannel(Channel::title2key(title));
	if (!channel)
		return ;
	channel->removeClient(client);
	client->removeChannel(channel);
	if (channel->isEmpty())
		_removeChannel(channel);
	(void) reason; // TODO depends on what the protocol needs...no idea right now
}

// -------------------------------------------------------------------------- //
// PRIVATE -- CHANNELS
// -------------------------------------------------------------------------- //

// Remove a channel by channel pointer.
void Server::_removeChannel(Channel* channel)
{
	_channels.erase(Channel::title2key(channel->getTitle()));
	delete channel;
}

// Remove a channel by title.
void Server::_removeChannel(const std::string& title)
{
	Channel* channel;

	channel = _getChannel(Channel::title2key(title));
	if (channel)
		_removeChannel(channel);
}

// Lookup a channel by name and return its pointer, or NULL if not found.
Channel* Server::_getChannel(const std::string& mapKey)
{
	// TODO is this safe enough? this function should only be called when we know the channel exists...
	std::map<std::string, Channel*>::iterator it;

	it = _channels.find(mapKey);
	if (it == _channels.end())
		return (NULL);
	else
		return (it->second);
}

// Lookup a channel by name, creating a new one with PW if it does not exist.
Channel* Server::_getOrCreateChannel(const std::string& title, const std::string& pw)
{
	Channel* channel;

	channel = _getChannel(title);
	if (!channel)
	{
		channel = new Channel(title, pw);
		_channels[title] = channel;
	}
	return (channel);
}

Channel* Server::_addChannel(const std::string& mapKey, const std::string& title, const std::string& pw)
{
	Channel* channel = new Channel(title, pw);
	_channels[mapKey] = channel;

	return (channel);
}

Channel* Server::_addChannel(const std::string& title, const std::string& pw)
{
	std::string mapKey = Channel::title2key(title);
	Channel* channel = new Channel(title, pw);
	_channels[mapKey] = channel;

	return (channel);
}

