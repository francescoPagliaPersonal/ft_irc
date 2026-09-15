/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server-Channels.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fpaglia <fpaglia@student.42vienna.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/17 13:55:04 by mweghofe          #+#    #+#             */
/*   Updated: 2026/09/02 17:21:58 by fpaglia          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_irc.hpp"
#include "Server.hpp"
#include "Channel.hpp"
#include "Response.hpp"
#include <cstddef>

// -------------------------------------------------------------------------- //
// INTERFACE -- CHANNELS
// -------------------------------------------------------------------------- //


Channel* Server::getChannelByTitle(std::string title) const
{
	return _getChannel(Channel::title2key(title));
}

// Remove CLIENT from CHANNEL, delete empty CHANNEL, export members to CONTACTS.
// Announce an automatic operator promotion so clients (irssi) update their UI.
void Server::removeClientFromChannel(Client* client, Channel& channel, std::set<Client*>* contacts)
{
	Client* promoted = channel.removeClient(client);
	if (channel.isEmpty())
	{
		_deleteChannel(&channel);
	}
	else
	{
		channel.pushMembersToSet(contacts);
		if (promoted != NULL)
		{
			broadcast(&channel, ":" + Response::getServerName()
				+ " MODE " + channel.getTitle()
				+ " +o " + promoted->getNick() + CRLF);
		}
	}
	client->removeChannel((&channel));
}

// Remove CLIENT from all registered channels, collect members in set CONTACTS.
void Server::removeClientFromAllChannels(Client * client, std::set<Client*>* contacts)
{
	std::deque<Channel*> joinedChannels;
	std::deque<Channel*>::iterator it;
	joinedChannels = client->getChannelsList();
	for (it = joinedChannels.begin(); it != joinedChannels.end(); ++it)
	{
		Channel & channel = *(*it);
		removeClientFromChannel(client, channel, contacts);
	}
}

// -------------------------------------------------------------------------- //
// PRIVATE -- CHANNELS
// -------------------------------------------------------------------------- //

// Delete a channel by channel pointer.
void Server::_deleteChannel(Channel* channel)
{
	_channels.erase(Channel::title2key(channel->getTitle()));
	delete channel;
}

// Lookup a channel by name and return its pointer, or NULL if not found.
Channel* Server::_getChannel(const std::string& mapKey) const
{
	// TODO is this safe enough? this function should only be called when we know the channel exists...
	std::map<std::string, Channel*>::const_iterator it;

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

