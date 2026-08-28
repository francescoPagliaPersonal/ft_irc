/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel-Get.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mweghofe <mweghofe@student.42vienna.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/17 17:12:46 by mweghofe          #+#    #+#             */
/*   Updated: 2026/08/28 10:22:40 by mweghofe         ###   ########.fr       */
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


bitMask Channel::getModes() const
{
	return _modes;
}
