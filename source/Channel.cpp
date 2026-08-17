/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mweghofe <mweghofe@student.42vienna.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/13 10:59:59 by mweghofe          #+#    #+#             */
/*   Updated: 2026/08/17 15:36:56 by mweghofe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Channel.hpp"

// -------------------------------------------------------------------------- //
// CUSTOM CTOR & DTOR
// -------------------------------------------------------------------------- //

Channel::Channel(const std::string& title, const std::string& pw)
	: _modes(0)
	, _userLimit(MAX_CHANNELUSERS)
	, _title(title)
	, _topic()
	, _password(pw)
	, _members()
{}

Channel::~Channel()
{}

// -------------------------------------------------------------------------- //
// OPERATION
// -------------------------------------------------------------------------- //

void Channel::addClient(const Client* client)
{
	_members[client] = 0;
}

void Channel::removeClient(const Client* client)
{
	_members.erase(client);
}

std::string Channel::getTitle() const
{
	return (_title);
}

std::string Channel::getTopic() const
{
	return (_topic);
}

std::string Channel::getPassword() const
{
	return (_password);
}

bool Channel::isEmpty() const
{
	if (_members.empty())
		return (true);
	return (false);
}

void Channel::setTopic(const std::string& topic)
{
	_topic = topic;
}

void Channel::setPassword(const std::string& password)
{
	_password = password;
}

void Channel::setLimit(t_uint userLimit)
{
	_userLimit = userLimit;
}

// -------------------------------------------------------------------------- //
// OCF - only declared, not defined, unusable
// -------------------------------------------------------------------------- //
