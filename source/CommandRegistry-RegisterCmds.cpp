/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CommandRegistry-RegisterCmds.cpp                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mweghofe <mweghofe@student.42vienna.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/12 11:41:20 by fpaglia           #+#    #+#             */
/*   Updated: 2026/08/16 17:42:04 by mweghofe         ###   ########.fr       */
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
}
