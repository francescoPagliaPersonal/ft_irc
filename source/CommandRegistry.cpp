/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CommandRegistry.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mweghofe <mweghofe@student.42vienna.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/09 18:00:05 by mweghofe          #+#    #+#             */
/*   Updated: 2026/08/17 16:52:38 by mweghofe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "CommandRegistry.hpp"
#include "IServerCtrl.hpp"

// -------------------------------------------------------------------------- //
// OPERATION
// -------------------------------------------------------------------------- //

int CommandRegistry::execute(IServerCtrl& srv, const Message& msg)
{
	Client *client = msg.sender;
	if (client == NULL)
		return (rfc::NOCONN);
	if (DEBUG)
		std::cout << "[FD " << client->getFD() << "] Executing Command <"
			<< msg.command << ">.\n";
	std::map<const std::string, const Command*>::iterator it;
	it = _commands.find(msg.command);	
	if ( it == _commands.end())
		return (rfc::BADCMD);     // ERR_UNKNOWNCOMMAND
	return (it->second->execute(srv, msg));
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
