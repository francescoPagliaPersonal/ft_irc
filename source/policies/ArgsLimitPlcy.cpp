/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ArgsLimitPlcy.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fpaglia <fpaglia@student.42vienna.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/16 14:48:41 by mweghofe          #+#    #+#             */
/*   Updated: 2026/08/17 14:55:00 by fpaglia          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_irc.hpp"
#include "policies/ArgsLimitPlcy.hpp"

// Validate that MSG has between _min and _max arguments.
int ArgsLimitPlcy::check(const Message & msg, IServerCtrl& srv) const
{
	(void) srv;
	int args = argCount(msg);
	if (args < _min)
		return (rfc::FEWPARAMS); // ERR_NEEDMOREPARAM
	// TODO needed? i couldn't find an official numeric for that
	// there only seems to be a 15 params rule for the msg, which might be a parsing case, not an execution one
	else if (args > _max)
		return rfc::MANYPARAMS; // ERR_TOOMANYPARAM
	return (rfc::OK);
}
