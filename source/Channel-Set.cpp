/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel-Set.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mweghofe <mweghofe@student.42vienna.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/17 17:13:02 by mweghofe          #+#    #+#             */
/*   Updated: 2026/08/17 17:41:43 by mweghofe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Channel.hpp"

// -------------------------------------------------------------------------- //
// SET...
// -------------------------------------------------------------------------- //

// Set the channel's topic.
void Channel::setTopic(const std::string& topic)
{
	_topic = topic;
}

// Set the channel's password.
void Channel::setPassword(const std::string& password)
{
	_password = password;
}

// Set the channel's user limit.
void Channel::setLimit(t_uint userLimit)
{
	_userLimit = userLimit;
}
