/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   AlreadyRegisteredPlcy.cpp                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mweghofe <mweghofe@student.42vienna.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/16 14:47:38 by mweghofe          #+#    #+#             */
/*   Updated: 2026/08/20 09:20:53 by mweghofe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "policies/AlreadyRegisteredPlcy.hpp"
#include "RFCnumeric.hpp"

// Validate the sender's registration state against the policy's _status.
int AlreadyRegisteredPlcy::check(const Message & msg, IServerCtrl& srv) const
{
	(void) srv;
	Client *client = msg.sender;
	if (_status)
	{
		if (client->getRegistrationFlags() != REG_DONE)
			return (rfc::NOTREGISTERED);
	}
	else if (client->getRegistrationFlags() == REG_DONE)
		return (rfc::ALREADYREGISTERED);
	return (rfc::OK);
}
