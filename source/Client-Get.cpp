/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client-Get.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fpaglia <fpaglia@student.42vienna.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/17 17:07:36 by mweghofe          #+#    #+#             */
/*   Updated: 2026/08/24 12:22:20 by fpaglia          ###   ########.fr       */
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

bool Client::getCap() const
{
	return (_capRequested);
}

std::deque<Channel*> Client::getChannelsList() const
{
	return _channels;
}
