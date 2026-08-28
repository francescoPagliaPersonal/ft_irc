/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_mode.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mweghofe <mweghofe@student.42vienna.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/27 16:57:46 by fpaglia           #+#    #+#             */
/*   Updated: 2026/08/28 10:22:31 by mweghofe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <cstddef>
#include <vector>
#include <iostream>

#include "Channel.hpp"
#include "Command.hpp"
#include "IServerCtrl.hpp"
#include "Client.hpp"
#include "Message.hpp"
#include "irc.hpp"
#include "Response.hpp"


rfc cmd_mode(IServerCtrl& srv, const Message& msg)
{
	Client* sender = msg.sender;
	
	
	if (msg.params.size() == 1)
	{
		std::cerr << __FUNCTION__ << " display Not handled yet. " << std::endl;
		return irc::UNKNOWNCOMMAND;
	}

	if (msg.params[0][0] == '#' || msg.params[0][0] == '&' )
	{
		Channel*	channel = srv.getChannelByTitle(msg.params[0]);
		bool		switcher = false;

		if (channel == NULL)
			return irc::NOSUCHCHANNEL;
		if (!channel->isChanOp(sender))
			return irc::CHANOPRIVSNEEDED;
		if (msg.params[1][0] != '+' &&  msg.params[1][0] != '-' )
			return irc::UNKNOWNMODE;
		switcher = msg.params[1][0] == '+' ? true : false;
		std::string modes = msg.params[1].substr(1);
		if (modes.find_first_of('i') != modes.npos)
			channel->setInvite(switcher);
		return irc::OK;
	}

	std::cerr << __FUNCTION__ << " single User Not handled yet. " << std::endl;

	return irc::OK;
}
