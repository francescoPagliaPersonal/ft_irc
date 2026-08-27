/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ArgsLimitPlcy.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mweghofe <mweghofe@student.42vienna.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/16 14:48:41 by mweghofe          #+#    #+#             */
/*   Updated: 2026/08/27 13:41:37 by mweghofe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "policies/ArgsLimitPlcy.hpp"
#include "irc.hpp"

// Validate that MSG has between _min and _max arguments.
rfc ArgsLimitPlcy::check(const Message & msg, IServerCtrl& srv) const
{
	(void) srv;
	int args = irc::argCount(msg);
	if (args < _min)
		return (irc::NEEDMOREPARAMS); // ERR_NEEDMOREPARAM
	// TODO needed? i couldn't find an official numeric for that
	// there only seems to be a 15 params rule for the msg, which might be a parsing case, not an execution one
	else if (args > _max)
		return irc::MANYPARAMS; // ERR_TOOMANYPARAM
	return (irc::OK);
}
