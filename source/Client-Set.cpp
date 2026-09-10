/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client-Set.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mweghofe <mweghofe@student.42vienna.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/17 17:08:53 by mweghofe          #+#    #+#             */
/*   Updated: 2026/09/10 17:50:22 by mweghofe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Client.hpp"

// -------------------------------------------------------------------------- //
// SET...
// -------------------------------------------------------------------------- //

// Register the given FLAGS bits; returns false if they were already set.
bool Client::setRegistrationFlags(int flags)
{
	if (!(_registrationFlags & flags))
	{
		_registrationFlags |= flags;
		return true;
	}
	return false;
}

// Set Client's nick name.
void Client::setNick(const std::string & str)
{
	_nick = str;
}


// Set Client's user name.
void Client::setUserName(const std::string & str)
{
	_userName = str; 
}


// Set Client's real name.
void Client::setRealName(const std::string & str)
{
	_realName = str; 
}

// Record that client requested CAP negotiation.
void Client::setCap(bool requested)
{
	_capRequested = requested;
}

void Client::setQuit(bool connected)
{
	_hasQuit = connected;
}

void Client::setLastMsgTime(std::time_t now)
{
	_lastMsgTime = now;
}

void Client::resetSpamCount()
{
	_spamCount = 0;
}

void Client::incrementSpamCount()
{
	_spamCount++;
}

void Client::setRemove(bool val)
{
	_toBeRemoved = val;
}

void Client::incrementPingCount()
{
	_pingCount++;
}

void Client::resetPingCount()
{
	_pingCount = 0;
}
