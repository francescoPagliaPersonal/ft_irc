/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel-Get.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mweghofe <mweghofe@student.42vienna.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/17 17:12:46 by mweghofe          #+#    #+#             */
/*   Updated: 2026/09/16 15:07:39 by mweghofe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Channel.hpp"

// -------------------------------------------------------------------------- //
// GET...
// -------------------------------------------------------------------------- //

// Return the channel's title.
std::string Channel::getTitle() const
{
	return (_title);
}

// Return the channel's topic.
std::string Channel::getTopic() const
{
	return (_topic);
}

// Return the channel's password.
std::string Channel::getPassword() const
{
	return (_password);
}

std::map<Client *, bitMask> Channel::getMembersMap() const
{
	return (_members);
}

irc::uint Channel::getLimit() const
{
	return (_userLimit);
}

bitMask Channel::getModes() const
{
	return _modes;
}

bool Channel::isFounder(Client* client) const
{
	// only called when it is guaranteed that the client is a member
	if (_members.at(client) & US_FOUNDER)
		return (true);
	return (false);
}
