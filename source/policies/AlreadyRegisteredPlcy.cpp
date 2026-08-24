/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   AlreadyRegisteredPlcy.cpp                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mweghofe <mweghofe@student.42vienna.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/16 14:47:38 by mweghofe          #+#    #+#             */
/*   Updated: 2026/08/24 18:15:53 by mweghofe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_irc.hpp"
#include "irc.hpp"
#include "policies/AlreadyRegisteredPlcy.hpp"

// Validate the sender's registration state against the policy's _status.
rfc AlreadyRegisteredPlcy::check(const Message & msg, IServerCtrl& srv) const
{
	(void) srv;
	Client *client = msg.sender;
	if (_status)
	{
		if (client->getRegistrationFlags() != REG_DONE)
			return (irc::NOTREG);
	}
	else if (client->getRegistrationFlags() == REG_DONE)
		return (irc::ALREADYREG);
	return (irc::OK);
}
