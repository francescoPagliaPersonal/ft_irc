/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ArgsLimitPlcy.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mweghofe <mweghofe@student.42vienna.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/16 14:48:41 by mweghofe          #+#    #+#             */
/*   Updated: 2026/08/16 14:50:44 by mweghofe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "policies/ArgsLimitPlcy.hpp"

int ArgsLimitPlcy::check(const Message & msg, IServerCtrl& srv) const
{
	Client *client = msg.sender;
	if (client == NULL)
		return 3; //ERR_HANGHUP
	(void) srv;
	int args = argCount(msg);
	if (args < _min)
		return 1; // ERR_NEEDMOREPARAM
	else if (args > _max)
		return 2; // ERR_TOOMANYPARAM
	return 0;
}
