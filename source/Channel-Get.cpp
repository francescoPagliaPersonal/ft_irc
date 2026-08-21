/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel-Get.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fpaglia <fpaglia@student.42vienna.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/17 17:12:46 by mweghofe          #+#    #+#             */
/*   Updated: 2026/08/21 16:58:17 by fpaglia          ###   ########.fr       */
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
