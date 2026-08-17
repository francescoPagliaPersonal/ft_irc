/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server-Channels.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mweghofe <mweghofe@student.42vienna.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/17 13:55:04 by mweghofe          #+#    #+#             */
/*   Updated: 2026/08/18 08:58:53 by mweghofe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"
#include "Channel.hpp"

// -------------------------------------------------------------------------- //
// INTERFACE -- CHANNELS
// -------------------------------------------------------------------------- //

// Add CLIENT to the channel CHNAME, creating it first if it does not
// exist yet; the channel password must match PW.
void Server::addToChannel(Client* client,
						  const std::string& title,
						  const std::string& pw = "")
{
	Channel* channel = _getOrCreateChannel(title, pw);
	// TODO does the client or the server check IF client is a member?
	if (client->isChannelMember(channel))
	{
		std::cout << "[Info] Client is already member of that channel - consequence not handled yet.\n";
		return ;
	}
	if (channel->getPassword() == pw)
	{
		channel->addClient(client);
		client->addChannel(channel);
	}
	else // TODO needs some proper code or just return false and cmd must send different reply
	{
		std::cout << "[Info] Channel password incorrect - consequence not handled yet.\n";
	}
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

// Send MSG to all members of the channel CHNAME, optionally excluding SENDER.
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

// Add a new channel with TITLE.
void Server::_addChannel(const std::string& title)
{
	Channel* newCh = new Channel(title, "");
	_channels[title] = newCh;
	if (DEBUG)
		std::cout << "[Channel] '" << newCh->getTitle() << "' added.\n";
}

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
	std::map<std::string, Channel*>::iterator it;

	it = _channels.find(title);
	if (it == _channels.end())
		return ;
	channel = it->second;
	_channels.erase(title);
	delete channel;
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
	std::map<std::string, Channel*>::iterator it;

	it = _channels.find(title);
	if (it == _channels.end())
	{
		channel = new Channel(title, pw);
		_channels[title] = channel;
	}
	else
		channel = it->second;
	return (channel);
}
