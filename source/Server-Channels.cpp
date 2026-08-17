/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server-Channels.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mweghofe <mweghofe@student.42vienna.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/17 13:55:04 by mweghofe          #+#    #+#             */
/*   Updated: 2026/08/17 15:04:39 by mweghofe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"
#include "Channel.hpp"

// -------------------------------------------------------------------------- //
// PUBLIC
// -------------------------------------------------------------------------- //

void Server::addToChannel(Client* client,
						  const std::string& chName,
						  const std::string& pw = NULL)
{
	// TODO does the client or the server check IF client is a member?
	Channel* channel = _getOrCreateChannel(chName, pw);
	channel->addClient(client);
	client->addChannel(channel);
}

void Server::removeFromChannel(Client* client, const std::string& chName,
							   const std::string& msg)
{
	// TODO does the client or the server check IF client is a member?
	Channel* channel = _getChannel(chName);
	channel->removeClient(client);
	client->removeChannel(channel);
	(void) msg; // TODO depends on what the protocol needs...no idea right now
}

void Server::broadcastToChannel(const std::string& chName,
						const std::string& msg,
						Client* sender = NULL)
{
	(void) chName;
	(void) msg;
	(void) sender;
	std::cout << "[Info] " << __FUNCTION__ << " is not implemented.\n";
}

// -------------------------------------------------------------------------- //
// PRIVATE
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
	Channel* channel = _channels[title];
	_channels.erase(title);
	delete channel;
}

Channel* Server::_getChannel(const std::string& chName)
{
	// TODO is it safe? this is only for when we know the channel must exist
	return (_channels[chName]);
}

Channel* Server::_getOrCreateChannel(const std::string& chName, const std::string& pw)
{
	Channel* channel;
	std::map<std::string, Channel*>::iterator it;

	it = _channels.find(chName);
	if (it == _channels.end())
		channel = new Channel(chName, pw);
	else
		channel = it->second;
	return (channel);
}
