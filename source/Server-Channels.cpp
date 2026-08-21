/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server-Channels.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fpaglia <fpaglia@student.42vienna.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/17 13:55:04 by mweghofe          #+#    #+#             */
/*   Updated: 2026/08/21 13:15:06 by fpaglia          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"
#include "Channel.hpp"
#include "ft_irc.hpp"
#include <cstddef>

// -------------------------------------------------------------------------- //
// INTERFACE -- CHANNELS
// -------------------------------------------------------------------------- //


Channel* Server::getChannelByTitle(const std::string & title)
{
	return _getChannel(title);
}

// Remove CLIENT from the channel CHNAME and clean up empty channels.
void Server::removeFromChannel(Client* client, const std::string& title,
							   const std::string& reason)
{
	// TODO does the client or the server check IF client is a member?
	Channel* channel = _getChannel(title);
	if (!channel)
		return ;
	channel->removeClient(client);
	client->removeChannel(channel);
	if (channel->isEmpty())
		_removeChannel(channel);
	(void) reason; // TODO depends on what the protocol needs...no idea right now
}

// Send MSG to all members of CHANNEL, optionally excluding SENDER.
void Server::broadcastToChannel(Channel* channel,
						const std::string& msg,
						Client* sender = NULL)
{
	broadcastToChannel(channel->getTitle(), msg, sender);
}

// Send MSG to all members of the channel TITLE, optionally excluding SENDER.
void Server::broadcastToChannel(const std::string& title,
						const std::string& msg,
						Client* sender = NULL)
{
	(void) title;
	(void) msg;
	(void) sender;
	std::cout << "[Info] " << __FUNCTION__ << " is not implemented.\n";
}

// -------------------------------------------------------------------------- //
// PRIVATE -- CHANNELS
// -------------------------------------------------------------------------- //

// Remove a channel by channel pointer.
void Server::_removeChannel(Channel* channel)
{
	_channels.erase(channel->getTitle());
	delete channel;
}

// Remove a channel by title.
void Server::_removeChannel(const std::string& title)
{
	Channel* channel;

	channel = _getChannel(title);
	if (channel)
		_removeChannel(channel);
}

// Lookup a channel by name and return its pointer, or NULL if not found.
Channel* Server::_getChannel(const std::string& title)
{
	// TODO is this safe enough? this function should only be called when we know the channel exists...
	std::map<std::string, Channel*>::iterator it;

	it = _channels.find(title);
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

Channel* Server::_newChannel(const std::string& title, const std::string& pw)
{
	Channel* channel = new Channel(title, pw);
	_channels[title] = channel;

	return (channel);
}
