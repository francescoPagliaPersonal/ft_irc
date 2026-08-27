/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CommandRegistry.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mweghofe <mweghofe@student.42vienna.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/09 18:00:05 by mweghofe          #+#    #+#             */
/*   Updated: 2026/08/27 13:15:58 by mweghofe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_irc.hpp"
#include "CommandRegistry.hpp"
#include "IServerCtrl.hpp"
#include "irc.hpp"
#include "Response.hpp"

// -------------------------------------------------------------------------- //
// OPERATION
// -------------------------------------------------------------------------- //

// Lookup MSG's command and execute it on SRV; handles numeric code replies.
void CommandRegistry::execute(IServerCtrl& srv, const Message& msg)
{
	Client *client = msg.sender;
	irc::rfc code;

	if (DEBUG)
		std::cout << "[FD " << client->getFD() << "] Executing Command <"
			<< msg.command << ">.\n";
	// look up if requested command exists on the server
	std::map<const std::string, const Command*>::iterator it;
	it = _commands.find(msg.command);
	// set code for unknown command
	if ( it == _commands.end())
		code = irc::UNKNOWNCOMMAND;     // ERR_UNKNOWNCOMMAND
	// execute known command
	else
		code = it->second->execute(srv, msg);
	// handle any registered error replies
	if (code != irc::OK)
		srv.sendMessage(client, Response::handleNumeric(msg, code));
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
