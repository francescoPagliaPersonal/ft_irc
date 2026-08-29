/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel-Operation.cpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mweghofe <mweghofe@student.42vienna.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/17 17:13:52 by mweghofe          #+#    #+#             */
/*   Updated: 2026/08/29 23:24:31 by mweghofe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Channel.hpp"
#include "irc.hpp"

// -------------------------------------------------------------------------- //
// OPERATION
// -------------------------------------------------------------------------- //

// Add CLIENT to the channel's member list.
void Channel::addClient(Client* client, bitMask privileges = 0)
{
	_members[client] = privileges;
}

// Remove CLIENT from the channel's member list.
void Channel::removeClient(Client* client)
{
	_members.erase(client);
	// TODO we need to rework the the whole ADD/REMOVE logic
	// TODO Server::removeFromChannel was supposed to be the interface; Server::addToChannel works fine
}

// Check if the channel has no members left.
bool Channel::isEmpty() const
{
	if (_members.empty())
		return (true);
	return (false);
}

std::string Channel::title2key(std::string title)
{
	title.erase(0,1);
	irc::allCaps(title);
	return title;
}

bool Channel::isChanOp(Client * client) const
{
	std::map<Client *, bitMask>::const_iterator it;
	it = _members.find(client);
	if (it == _members.end())
		return false;
	if (it->second & US_OPERATOR)
		return true;
	return false;
}

void Channel::invite(Client * client)
{
	_invites.push_back(client);
}

// Attempt to add each member of the channel to DEST.
void Channel::getMembers(std::set<Client*>* dest) const
{
	std::map<Client*, bitMask>::const_iterator it;
	for (it = _members.begin(); it != _members.end(); it++)
	{
		dest->insert(it->first);
	}
}
