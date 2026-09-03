/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client-Get.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mweghofe <mweghofe@student.42vienna.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/17 17:07:36 by mweghofe          #+#    #+#             */
/*   Updated: 2026/09/03 12:46:57 by mweghofe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Client.hpp"

// -------------------------------------------------------------------------- //
// GET...
// -------------------------------------------------------------------------- //

// Return Client FD.
int Client::getFD() const
{
	return (_fd);
}

// Return Client's registration flags bitmask.
int Client::getRegistrationFlags() const
{
	return (_registrationFlags);
}

// Return Client's nick name.
std::string Client::getNick() const
{
	return _nick;
}

// Return Client's user name.
std::string Client::getUserName() const
{
	return (_userName);
}

// Return Client's real name.
std::string Client::getRealName() const
{
	return (_realName);
}

// Return Client's host name (ie. IP address)
std::string Client::getHost() const
{
	return (_host);
}

// Return Client's full ID (NICK!USER@HOST)
std::string Client::getID() const
{
	return (std::string(_nick + "!" + _userName + "@" + _host));
}

bool Client::getCap() const
{
	return (_capRequested);
}

bool Client::hasQuit() const
{
	return (_hasQuit);
}

bool Client::isBufferOutFilled() const
{
	return (!_bufOUT.empty());
}

std::deque<Channel*> Client::getChannelsList() const
{
	return _channels;
}

bool Client::toBeKilled() const
{
	return (_toBeKilled);
}

std::size_t Client::inSize() const
{
	return (_bufIN.size());
}

std::size_t Client::outSize() const
{
	return (_bufOUT.size());
}
