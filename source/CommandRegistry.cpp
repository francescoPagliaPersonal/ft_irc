/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CommandRegistry.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mweghofe <mweghofe@student.42vienna.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/09 18:00:05 by mweghofe          #+#    #+#             */
/*   Updated: 2026/08/09 18:19:34 by mweghofe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "CommandRegistry.hpp"

// -------------------------------------------------------------------------- //
// OPERATION
// -------------------------------------------------------------------------- //

void CommandRegistry::registerCmd(const std::string& name, const Command* cmd)
{
	commands_.insert(std::make_pair(name, cmd));
}

void CommandRegistry::dispatch(Server& srv, Client& client, const Message& msg)
{
	(void) srv, (void) client, (void) msg;
}

// -------------------------------------------------------------------------- //
// OCF
// -------------------------------------------------------------------------- //

CommandRegistry::CommandRegistry()
	: commands_()
{}

CommandRegistry::CommandRegistry(const CommandRegistry& other)
	: commands_()
{
	(void) other;
}

CommandRegistry::~CommandRegistry()
{}

CommandRegistry CommandRegistry::operator=(const CommandRegistry& other)
{
	(void) other;
	return (*this);
}
