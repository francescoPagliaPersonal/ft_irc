/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CommandRegistry.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mweghofe <mweghofe@student.42vienna.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/09 18:00:05 by mweghofe          #+#    #+#             */
/*   Updated: 2026/08/16 12:37:33 by mweghofe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "CommandRegistry.hpp"

// -------------------------------------------------------------------------- //
// OPERATION
// -------------------------------------------------------------------------- //

int CommandRegistry::execute(Server& srv, const Message& msg)
{
	Client *client = msg.sender;
	if (client == NULL)
		return 1;
	std::map<const std::string, const Command*>::iterator it;
	it = _commands.find(msg.command);
	
	if ( it == _commands.end())
	{
		// FIXME pick a unified place to handle PROTOCOL ERROR CODES (see _executeCommands)
		std::cout << "[Warning] " << __FUNCTION__
				<< " found an unknown command. Code needs to handle that.\n";
		// std::string numeric(":CoolServ 421 :Command not found.\r\n");
		// std::cout << numeric << std::endl;
		// client->putReply2Buff(srv, numeric);
		return 421;     // ERR_UNKNOWNCOMMAND
	}
	it->second->execute(srv, msg); // FIXME forward protocol code
	return 0;
}

// -------------------------------------------------------------------------- //
// OCF
// -------------------------------------------------------------------------- //

CommandRegistry::CommandRegistry()
	: _commands()
{}

CommandRegistry::CommandRegistry(const CommandRegistry& other)
	: _commands()
{
	(void) other;
}

CommandRegistry::~CommandRegistry()
{
	std::map<const std::string, const Command*>::iterator it;
	for (it = _commands.begin(); it != _commands.end(); it++)
		delete it->second;
}

CommandRegistry CommandRegistry::operator=(const CommandRegistry& other)
{
	(void) other;
	return (*this);
}
