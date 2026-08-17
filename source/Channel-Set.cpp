/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel-Set.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mweghofe <mweghofe@student.42vienna.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/17 17:13:02 by mweghofe          #+#    #+#             */
/*   Updated: 2026/08/17 17:19:12 by mweghofe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Channel.hpp"

// -------------------------------------------------------------------------- //
// SET...
// -------------------------------------------------------------------------- //

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
