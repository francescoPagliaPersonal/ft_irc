/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bot_spam.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mweghofe <mweghofe@student.42vienna.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/11 17:47:14 by mweghofe          #+#    #+#             */
/*   Updated: 2026/09/11 22:29:27 by mweghofe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Command.hpp"
#include "IBot.hpp"
#include "Bot.hpp"
#include "irc.hpp"

void bot_spam(IBot& bot, const Message& msg, std::vector<std::string>& botcmds)
{
	(void) bot; (void) msg; (void) botcmds;
	// TODO get nick and stuff
	bot.sendMessage(std::string("PRIVMSG ") + SPAM_USER + " :" + SPAM_MSG_42 + CRLF);
	// TODO get channel and stuff
	// bot.sendMessage("PRIVMSG " + channel + " :" + SPAM_MSG_CH + CRLF);
}
