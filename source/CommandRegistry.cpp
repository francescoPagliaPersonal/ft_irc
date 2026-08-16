/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CommandRegistry.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mweghofe <mweghofe@student.42vienna.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/09 18:00:05 by mweghofe          #+#    #+#             */
/*   Updated: 2026/08/16 15:15:49 by mweghofe         ###   ########.fr       */
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
	std::map<const std::string, const Command*>::iterator it;
	it = _commands.find(msg.command);	
	if ( it == _commands.end())
		return (rfc::BADCMD);     // ERR_UNKNOWNCOMMAND
	return (it->second->execute(srv, msg));
}

bool CommandRegistry::handleProtocolErrors(IServerCtrl& srv, int numeric,
	const Message& msg)
{
	if (!msg.sender)
		return (false);
	Client& client = *msg.sender;
	// TODO requires a proper mechanism; some map perhaps with CODE + REPLY STRING
	switch (numeric)
	{
		case 0: break ;
		// case 371:
		// 	srv.sendMessage(client, ":CoolServ 371 :A policy has not been respected.\r\n");
		// 	break ;
		case rfc::BADCMD:
			srv.sendMessage(client, ":CoolServ 421 :Command not found.\r\n");
			break ;
		default:
			std::cout << "[Warning] " << __FUNCTION__
				<< " received a currently unknown protocol error: " << numeric
				<< std::endl;
	}
	return (true);
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
