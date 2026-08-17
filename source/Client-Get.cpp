/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client-Get.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mweghofe <mweghofe@student.42vienna.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/17 17:07:36 by mweghofe          #+#    #+#             */
/*   Updated: 2026/08/17 17:08:37 by mweghofe         ###   ########.fr       */
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

int Client::getRegistrationFlags() const
{
	return (_registrationFlags);
}

std::string Client::getNick() const
{
	return _nick;
}

std::string Client::getUserName() const
{
	return _userName;
}

std::string Client::getRealName() const
{
	return _realName;
}
