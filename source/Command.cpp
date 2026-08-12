/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Command.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mweghofe <mweghofe@student.42vienna.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/09 18:07:43 by mweghofe          #+#    #+#             */
/*   Updated: 2026/08/12 14:18:58 by mweghofe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Command.hpp"

// -------------------------------------------------------------------------- //
// CUSTOM CTOR & DTOR
// -------------------------------------------------------------------------- //

Command::Command(const std::string& name, command func)
	: _name(name)
	, _func(func)
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
	: _name()
	, _func()
{}

Command::Command(const Command& other)
	: _name()
	, _func()
{
	(void) other;
}

Command Command::operator=(const Command& other)
{
	(void) other;
	return (*this);
}
