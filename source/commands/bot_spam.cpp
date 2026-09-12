/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bot_spam.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mweghofe <mweghofe@student.42vienna.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/11 17:47:14 by mweghofe          #+#    #+#             */
/*   Updated: 2026/09/12 10:45:30 by mweghofe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Command.hpp"
#include "IBot.hpp"
#include "Bot.hpp"
#include "irc.hpp"

namespace
{
	void spamTarget(IBot& bot, const std::string& target, const std::string& text)
	{
		for (irc::uint8 i = 0; i < SPAM_COUNT; i++)
			bot.sendMessage("PRIVMSG " + target + " :" + text + CRLF);
	}
}
#include <iostream>
void bot_spam(IBot& bot, const Message& msg, std::vector<std::string>& botcmds)
{
	if (msg.params.empty())
		return ;
	// 1) spam the current channel
	if (botcmds.size() == 1 && (msg.params[0][0] == '#' || msg.params[0][0] == '&'))
		spamTarget(bot, msg.params[0], SPAM_MSG_CH);
	else if (botcmds.size() > 1)
	{
		// 2) spam the target channel
		if (botcmds[1][0] == '#' || botcmds[1][0] == '&')
			spamTarget(bot, botcmds[1], SPAM_MSG_CH);
		// 3) spam the target user
		else
			spamTarget(bot, botcmds[1], SPAM_MSG_42);
	}
}
