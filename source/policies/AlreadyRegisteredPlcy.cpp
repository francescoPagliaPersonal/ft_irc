/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   AlreadyRegisteredPlcy.cpp                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fpaglia <fpaglia@student.42vienna.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/16 14:47:38 by mweghofe          #+#    #+#             */
/*   Updated: 2026/08/17 14:53:47 by fpaglia          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_irc.hpp"
#include "policies/AlreadyRegisteredPlcy.hpp"

// Validate the sender's registration state against the policy's _status.
int AlreadyRegisteredPlcy::check(const Message & msg, IServerCtrl& srv) const
{
	(void) srv;
	Client *client = msg.sender;
	if (_status)
	{
		if (client->getRegistrationFlags() != REG_DONE)
			return (rfc::NOTREG);
	}
	else if (client->getRegistrationFlags() == REG_DONE)
		return (rfc::ALREADYREG);
	return (rfc::OK);
}
