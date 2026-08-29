/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel-Set.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mweghofe <mweghofe@student.42vienna.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/17 17:13:02 by mweghofe          #+#    #+#             */
/*   Updated: 2026/08/29 12:41:09 by mweghofe         ###   ########.fr       */
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
	if (userLimit <= MAX_CHANNELUSERS)
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

void Channel::setTopicFlag(bool switcher)
{
	if (switcher)
		_modes |= CH_TOPIC;
	else
		_modes &= ~CH_TOPIC;
}

void Channel::setOperator(bool switcher, Client* client)
{
	std::cerr << "[Warning] " << __FUNCTION__ << "not defined yet.\n";
	(void) switcher; (void) client;
}

void Channel::addOperator(Client* client)
{
	std::map<Client*, bitMask>::iterator it;
	it = _members.find(client);
	if (it == _members.end())
		return ;
	it->second |= US_OPERATOR;
}

void Channel::removeOperator(Client* client)
{
	std::map<Client*, bitMask>::iterator it;
	it = _members.find(client);
	if (it == _members.end())
		return ;
	it->second &= ~US_OPERATOR;
}
