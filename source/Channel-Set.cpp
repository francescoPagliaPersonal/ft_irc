/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel-Set.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mweghofe <mweghofe@student.42vienna.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/17 17:13:02 by mweghofe          #+#    #+#             */
/*   Updated: 2026/09/16 18:39:24 by mweghofe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Channel.hpp"
#include "ft_irc.hpp"

// -------------------------------------------------------------------------- //
// SET...
// -------------------------------------------------------------------------- //

// Set the channel's topic.
void Channel::setTopic(const std::string& topic)
{
	_topic = topic;
}

// Set the channel's password and mode flag.
void Channel::setPassword(const std::string& password)
{
	if ((_modes & CH_PASSWORD) != 0)
		return ;
	_password = password;
	_modes |= CH_PASSWORD;
}

// Remove th channel's password and mode flag.
void Channel::removePassword()
{
	_password.clear();
	_modes &= ~CH_PASSWORD;
}

// Set the channel's user limit and mode flag.
void Channel::setLimit(irc::uint userLimit)
{
	if (userLimit == 0)
		return ;
	else if (userLimit <= MAX_CHANNELUSERS)
		_userLimit = userLimit;
	else
		_userLimit = MAX_CHANNELUSERS;
	_modes |= CH_LIMIT;
}

// Remove the channel's user limit and mode flag.
void Channel::removeLimit()
{
	_userLimit = MAX_CHANNELUSERS;
	_modes &= ~CH_LIMIT;
}

void Channel::setInvite(bool switcher)
{
	if (switcher)
		_modes |= CH_INVITE;
	else
		_modes &= ~CH_INVITE;
}

void Channel::setTopicFlag(bool switcher)
{
	if (switcher)
		_modes |= CH_TOPIC;
	else
		_modes &= ~CH_TOPIC;
}

/*
	Toggle operator flag on a member CLIENT of the current channel.
	The caller guarantees, that CLIENT IS on the channel.
	(irc::USERNOTINCHANNEL already triggered before.
*/
bool Channel::setOperator(bool switcher, Client* client)
{
	std::map<Client*, bitMask>::iterator it;
	it = _members.find(client);
	if (switcher)
	{
		if (!(it->second & US_OPERATOR))
		{
			it->second |= US_OPERATOR;
			++_chanOps;
		}
		else
			return (false);
	}
	else if (it->second & US_OPERATOR)
	{
		it->second &= ~US_OPERATOR;
		--_chanOps;
	}
	else
		return (false);
	return (true);
}
