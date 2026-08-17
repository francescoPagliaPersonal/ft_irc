/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client-Set.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mweghofe <mweghofe@student.42vienna.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/17 17:08:53 by mweghofe          #+#    #+#             */
/*   Updated: 2026/08/17 17:09:55 by mweghofe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Client.hpp"

// -------------------------------------------------------------------------- //
// SET...
// -------------------------------------------------------------------------- //

bool Client::setRegistrationFlags(int flags)
{
	if (!(_registrationFlags & flags))
	{
		_registrationFlags |= flags;
		return true;
	}
	return false;
}

void Client::setNick(const std::string & str)
{
	_nick = str;
}


void Client::setUserName(const std::string & str)
{
	_userName = str; 
}


void Client::setRealName(const std::string & str)
{
	_realName = str; 
}
