/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CommandRegistry-RegisterCmds.cpp                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mweghofe <mweghofe@student.42vienna.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/12 11:41:20 by fpaglia           #+#    #+#             */
/*   Updated: 2026/08/29 21:53:45 by mweghofe         ###   ########.fr       */
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
	ping->addPolicy(new ArgsLimitPlcy(1, 2));
	_commands[ping->getName()] = ping;

	Command *join = new Command("JOIN", cmd_join);
	join->addPolicy(new AlreadyRegisteredPlcy(true));
	join->addPolicy(new ArgsLimitPlcy(1, 2));
	_commands[join->getName()] = join;
	
	Command	*privmsg = new Command("PRIVMSG", cmd_privmsg);
	privmsg->addPolicy(new AlreadyRegisteredPlcy(true));
	privmsg->addPolicy(new ArgsLimitPlcy(2, 2));
	_commands[privmsg->getName()] = privmsg;

	Command	*invite = new Command("INVITE", cmd_invite);
	invite->addPolicy(new AlreadyRegisteredPlcy(true));
	invite->addPolicy(new ArgsLimitPlcy(2, 2));
	_commands[invite->getName()] = invite;

	Command *mode = new Command("MODE", cmd_mode);
	mode->addPolicy(new AlreadyRegisteredPlcy(true));
	// 8 params should allow each setting to be set in one call
	mode->addPolicy(new ArgsLimitPlcy(1, 8));
	_commands[mode->getName()] = mode;
}
