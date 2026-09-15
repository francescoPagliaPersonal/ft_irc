/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   AlreadyRegisteredPlcy.cpp                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mweghofe <mweghofe@student.42vienna.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/16 14:47:38 by mweghofe          #+#    #+#             */
/*   Updated: 2026/09/11 16:52:19 by mweghofe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_irc.hpp"
#include "irc.hpp"
#include "policies/AlreadyRegisteredPlcy.hpp"

// Validate the sender's registration state against the policy's _status.
rfc AlreadyRegisteredPlcy::check(const Message & msg) const
{
	Client *client = msg.sender;
	if (_status)
	{
		if (client->getRegistrationFlags() != REG_DONE)
			return (irc::NOTREGISTERED);
	}
	else if (client->getRegistrationFlags() == REG_DONE)
		return (irc::ALREADYREGISTERED);
	return (irc::OK);
}
