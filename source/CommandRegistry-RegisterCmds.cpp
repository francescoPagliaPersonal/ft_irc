/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CommandRegistry-RegisterCmds.cpp                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mweghofe <mweghofe@student.42vienna.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/12 11:41:20 by fpaglia           #+#    #+#             */
/*   Updated: 2026/09/16 12:43:57 by mweghofe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "CommandRegistry.hpp"
#include "Command.hpp"
#include "policies/ArgsLimitPlcy.hpp"
#include "policies/AlreadyRegisteredPlcy.hpp"

/*
	IMPORTANT FOR FUTURE COMMANDS
	- full workflow for checking AlreadyRegisteredPlcy is implemented
	- thus AFTER registration is done, other CMDs require that require a
	  registered user MUST be set to TRUE
	example:
		join->addPolicy(new AlreadyRegisteredPlcy(true));
*/

#ifndef BONUS

// Register all known commands with their policies.
void CommandRegistry::registerCmds()
{
	Command	*cap = new Command("CAP", cmd_cap);
	cap->addPolicy(new ArgsLimitPlcy(1, 15));
	_commands[cap->getName()] = cap;

	Command	*pass = new Command("PASS", cmd_pass);
	pass->addPolicy(new AlreadyRegisteredPlcy(false));
	pass->addPolicy(new ArgsLimitPlcy(1, 1));
	_commands[pass->getName()] = pass;

	Command	*nick = new Command("NICK", cmd_nick);
	nick->addPolicy(new ArgsLimitPlcy(1, 2));
	_commands[nick->getName()] = nick;

	Command	*user = new Command("USER", cmd_user);
	user->addPolicy(new AlreadyRegisteredPlcy(false));
	user->addPolicy(new ArgsLimitPlcy(4, 4));
	_commands[user->getName()] = user;

	Command *ping = new Command("PING", cmd_ping);
	ping->addPolicy(new ArgsLimitPlcy(1, 2)); // FIXME 409 ERR_NOORIGIN of no ARG given, not param error
	_commands[ping->getName()] = ping;

	Command *join = new Command("JOIN", cmd_join);
	join->addPolicy(new AlreadyRegisteredPlcy(true));
	join->addPolicy(new ArgsLimitPlcy(1, 2));
	_commands[join->getName()] = join;
	
	Command	*privmsg = new Command("PRIVMSG", cmd_privmsg);
	privmsg->addPolicy(new AlreadyRegisteredPlcy(true));
	privmsg->addPolicy(new ArgsLimitPlcy(0, 2));
	_commands[privmsg->getName()] = privmsg;

	Command	*invite = new Command("INVITE", cmd_invite);
	invite->addPolicy(new AlreadyRegisteredPlcy(true));
	invite->addPolicy(new ArgsLimitPlcy(2, 2));
	_commands[invite->getName()] = invite;

	Command *quit = new Command("QUIT", cmd_quit);
	quit->addPolicy(new AlreadyRegisteredPlcy(true));
	quit->addPolicy(new ArgsLimitPlcy(0, 1));
	_commands[quit->getName()] = quit;

	Command *mode = new Command("MODE", cmd_mode);
	mode->addPolicy(new AlreadyRegisteredPlcy(true));
	mode->addPolicy(new ArgsLimitPlcy(1, 9));
	_commands[mode->getName()] = mode;

	Command *topic = new Command("TOPIC", cmd_topic);
	topic->addPolicy(new AlreadyRegisteredPlcy(true));
	topic->addPolicy(new ArgsLimitPlcy(1, 2));
	_commands[topic->getName()] = topic;
	
	Command *kick = new Command("KICK", cmd_kick);
	kick->addPolicy(new AlreadyRegisteredPlcy(true));
	kick->addPolicy(new ArgsLimitPlcy(2, 3));
	_commands[kick->getName()] = kick;

	Command *part = new Command("PART", cmd_part);
	part->addPolicy(new AlreadyRegisteredPlcy(true));
	part->addPolicy(new ArgsLimitPlcy(1, 2));
	_commands[part->getName()] = part;

	Command *pong = new Command("PONG", cmd_pong);
	pong->addPolicy(new AlreadyRegisteredPlcy(true));
	pong->addPolicy(new ArgsLimitPlcy(0, 2)); // TODO that max value... don't like it
	_commands[pong->getName()] = pong;
}

# else

void CommandRegistry::registerBotCmds()
{
	Command *help = new Command("!help", bot_help);
	_commands[help->getName()] = help;

	Command *spam = new Command("!spam", bot_spam);
	_commands[spam->getName()] = spam;

	Command *mirror = new Command("!mirror", bot_mirror);
	_commands[mirror->getName()] = mirror;

	Command *quote = new Command("!quote", bot_quote);
	_commands[quote->getName()] = quote;
}

#endif
