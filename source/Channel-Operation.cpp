/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel-Operation.cpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mweghofe <mweghofe@student.42vienna.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/17 17:13:52 by mweghofe          #+#    #+#             */
/*   Updated: 2026/09/18 08:38:19 by mweghofe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Channel.hpp"
#include "Client.hpp"
#include "irc.hpp"

// -------------------------------------------------------------------------- //
// OPERATION
// -------------------------------------------------------------------------- //

// Add CLIENT to the channel's member list.
void Channel::addClient(Client* client, bitMask privileges = 0)
{
	_members[client] = privileges;
	if (privileges & US_OPERATOR)
		++_chanOps;
}

// Promote the first remaining member when the channel has no operator.
Client* Channel::ensureChanOp()
{
	if (_members.empty() || _chanOps != 0)
		return (NULL);
	_members.begin()->second |= US_OPERATOR;
	++_chanOps;
	return (_members.begin()->first);
}

// Remove CLIENT from the channel's member list.
// Promote the first remaining member to operator when the last
// operator is gone; return the promoted client (NULL if none).
Client* Channel::removeClient(Client* client)
{
	std::map<Client *, bitMask>::iterator member;

	member = _members.find(client);
	if (member == _members.end())
		return (NULL);
	if (member->second & US_OPERATOR)
		--_chanOps;
	_members.erase(client);
	return (ensureChanOp());
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

void Channel::pushMembersToSet(std::set<Client *> * setName) const
{
	std::map<Client*, bitMask>::const_iterator it;
	for (it = _members.begin(); it != _members.end(); it++)
	{
		setName->insert(it->first);
	}
}
