/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bot_help.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mweghofe <mweghofe@student.42vienna.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/11 16:56:33 by mweghofe          #+#    #+#             */
/*   Updated: 2026/09/15 15:04:08 by mweghofe         ###   ########.fr       */
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
	std::string recipient;
	if (msg.params[0][0] == '#' || msg.params[0][0] == '&')
		recipient = msg.params[0];
	else
		recipient = msg.prefix.substr(0, msg.prefix.find('!'));
	bot.sendMessage("PRIVMSG " + recipient + " :" HELP_L1 + CRLF);
	bot.sendMessage("PRIVMSG " + recipient + " :" HELP_L2 + CRLF);
	bot.sendMessage("PRIVMSG " + recipient + " :" HELP_L3 + CRLF);
	bot.sendMessage("PRIVMSG " + recipient + " :" HELP_L4 + CRLF);
	bot.sendMessage("PRIVMSG " + recipient + " :" HELP_L5 + CRLF);
}
