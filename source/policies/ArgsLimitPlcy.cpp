/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ArgsLimitPlcy.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fpaglia <fpaglia@student.42vienna.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/16 14:48:41 by mweghofe          #+#    #+#             */
/*   Updated: 2026/09/16 10:26:03 by fpaglia          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "policies/ArgsLimitPlcy.hpp"
#include "irc.hpp"

// Validate that MSG has between _min and _max arguments.
rfc ArgsLimitPlcy::check(const Message & msg) const
{
	int args = msg.argCount();
	if (args < _min)
		return (irc::NEEDMOREPARAMS); // ERR_NEEDMOREPARAM
	// TODO needed? i couldn't find an official numeric for that
	// there only seems to be a 15 params rule for the msg, which might be a parsing case, not an execution one
	else if (args > _max)
		return irc::MANYPARAMS; // ERR_TOOMANYPARAM
	return (irc::OK);
}
