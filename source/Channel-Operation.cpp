/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel-Operation.cpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mweghofe <mweghofe@student.42vienna.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/17 17:13:52 by mweghofe          #+#    #+#             */
/*   Updated: 2026/08/19 18:33:40 by fpaglia          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Channel.hpp"

// -------------------------------------------------------------------------- //
// OPERATION
// -------------------------------------------------------------------------- //

// Add CLIENT to the channel's member list.
void Channel::addClient(Client* client)
{
	_members[client] = 0;
}

// Remove CLIENT from the channel's member list.
void Channel::removeClient(Client* client)
{
	_members.erase(client);
}

// Check if the channel has no members left.
bool Channel::isEmpty() const
{
	if (_members.empty())
		return (true);
	return (false);
}
