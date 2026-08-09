/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Command.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mweghofe <mweghofe@student.42vienna.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/09 18:07:43 by mweghofe          #+#    #+#             */
/*   Updated: 2026/08/09 18:19:34 by mweghofe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Command.hpp"

// -------------------------------------------------------------------------- //
// CUSTOM CTOR & DTOR
// -------------------------------------------------------------------------- //

Command::Command(const std::string& name, command func)
	: name_(name)
	, func_(func)
{
	// TODO fill me up
}

Command::~Command()
{}

// -------------------------------------------------------------------------- //
// OPERATION
// -------------------------------------------------------------------------- //

void Command::getName() const
{}

void Command::addPolicy()
{}

void Command::execute(Server& srv, Client& client, const Message& msg)
{
	(void) srv, (void) client, (void) msg;
}

// -------------------------------------------------------------------------- //
// OCF
// -------------------------------------------------------------------------- //

Command::Command()
{}

Command::Command(const Command& other)
{
	(void) other;
}

Command Command::operator=(const Command& other)
{
	(void) other;
	return (*this);
}
