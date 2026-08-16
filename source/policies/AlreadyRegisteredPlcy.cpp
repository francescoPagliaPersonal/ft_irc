/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   AlreadyRegisteredPlcy.cpp                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mweghofe <mweghofe@student.42vienna.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/16 14:47:38 by mweghofe          #+#    #+#             */
/*   Updated: 2026/08/16 17:42:04 by mweghofe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_irc.hpp"
#include "policies/AlreadyRegisteredPlcy.hpp"

int AlreadyRegisteredPlcy::check(const Message & msg, IServerCtrl& srv) const
{
	(void) srv;
	Client *client = msg.sender;
	if (client == NULL)
		return (rfc::NOCONN); //ERR_HANGHUP
	if (_status)
	{
		if (client->getRegistrationFlags() != REG_DONE)
			return (rfc::NOTREG);
	}
	else if (client->getRegistrationFlags() == REG_DONE)
		return (rfc::ALREADYREG);
	return (rfc::OK);
}
