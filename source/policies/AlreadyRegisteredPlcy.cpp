/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   AlreadyRegisteredPlcy.cpp                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mweghofe <mweghofe@student.42vienna.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/16 14:47:38 by mweghofe          #+#    #+#             */
/*   Updated: 2026/08/16 14:50:39 by mweghofe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "policies/AlreadyRegisteredPlcy.hpp"

int AlredyRegisteredPlcy::check(const Message & msg, IServerCtrl& srv) const
{
	(void) srv;
	Client *client = msg.sender;
	if (client == NULL)
		return 3; //ERR_HANGHUP
	if (_status && client->getRegistrationFlags() != REG_DONE)
		return 1; // ERR_ALREADYREGISTERED
	return 0;
}
