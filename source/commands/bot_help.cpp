/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bot_help.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mweghofe <mweghofe@student.42vienna.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/11 16:56:33 by mweghofe          #+#    #+#             */
/*   Updated: 2026/09/12 14:05:00 by mweghofe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Command.hpp"
#include "IBot.hpp"
#include "irc.hpp"

void bot_help(IBot& bot, const Message& msg, std::vector<std::string>& botcmds)
{
	(void) botcmds;
	if (msg.params.empty())
		return ;
	bot.sendMessage("PRIVMSG " + msg.params[0] + " :" HELP_L1 + CRLF);
	bot.sendMessage("PRIVMSG " + msg.params[0] + " :" HELP_L2 + CRLF);
	bot.sendMessage("PRIVMSG " + msg.params[0] + " :" HELP_L3 + CRLF);
	bot.sendMessage("PRIVMSG " + msg.params[0] + " :" HELP_L4 + CRLF);
	bot.sendMessage("PRIVMSG " + msg.params[0] + " :" HELP_L5 + CRLF);
}
