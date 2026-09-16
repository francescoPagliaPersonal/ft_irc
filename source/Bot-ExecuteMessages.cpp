/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Bot-ExecuteMessages.cpp                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fpaglia <fpaglia@student.42vienna.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/12 14:52:56 by mweghofe          #+#    #+#             */
/*   Updated: 2026/09/16 10:18:16 by fpaglia          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Bot.hpp"
#include "ft_irc.hpp"
#include "CommandRegistry.hpp"

// Controls internal actions and custom command execution.
void Bot::_executeMessages()
{
	if (DEBUG && !_msgsQueue.empty())
		std::cout << "[Bot] Processing message queue with "
			<< _msgsQueue.size() << " messages...\n";
	while (!_msgsQueue.empty())
	{
		Message& msg = _msgsQueue.front();
		// 1) extract custom commands and args from trailing
		std::vector<std::string> botcmds = irc::strSplit(msg.getTrailing(), ' ', false);
		// 2) evaluate and execute custom commands
		if (!_cmdReg.execute(*this, msg, botcmds))
		{
			// 3) action needed to confirm server registration and finish setup
			if (!_joinedServer)
			{
				if (msg.command == "433")
				{
					_keepRunning = false;
					std::cout << "[Bot] Another bot is already connected.\n";
				}
				else if (msg.command == "001")
					_joinedServer = true;
			}
			// 4) other supported actions
			else if (_mirrorMsg && msg.command == "PRIVMSG")
				_mirrorMessage(msg);
			else if (msg.command == "INVITE")
				_processInvite(msg);
			else if (msg.command == "JOIN")
				_welcomeUser(msg);
			else if (msg.command == "PING")
				_pong(msg);
		}
		_msgsQueue.pop_front();
	}
}
