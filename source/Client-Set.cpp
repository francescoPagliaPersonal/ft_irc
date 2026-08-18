/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client-Set.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mweghofe <mweghofe@student.42vienna.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/17 17:08:53 by mweghofe          #+#    #+#             */
/*   Updated: 2026/08/17 17:42:15 by mweghofe         ###   ########.fr       */
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
