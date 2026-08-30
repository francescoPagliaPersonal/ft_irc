/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Command.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mweghofe <mweghofe@student.42vienna.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/09 18:07:43 by mweghofe          #+#    #+#             */
/*   Updated: 2026/08/30 10:04:59 by mweghofe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Command.hpp"
#include "IPolicy.hpp"
#include "irc.hpp"

// -------------------------------------------------------------------------- //
// CUSTOM CTOR & DTOR
// -------------------------------------------------------------------------- //

// Custom constructor to create a command with NAME and its handler FUNC.
Command::Command(const std::string& name, command func)
	: _name(name)
	, _func(func)
	, _policies()
{}

// Free all attached policies.
Command::~Command()
{
	std::vector<IPolicy*>::size_type i;
	for (i = 0; i < _policies.size(); ++i)
	{
		delete _policies[i];
	}
}

Command::Data::Data(IServerCtrl& s, const Message& m, Client* c)
	: srv(s)
	, msg(m)
	, client(c)
	, channel(NULL)
	, modeChOPadd()
	, modeChOPrem()
{}

// -------------------------------------------------------------------------- //
// OPERATION
// -------------------------------------------------------------------------- //

// Return the command's name.
std::string Command::getName() const
{
	return (_name);
}

// Attach a policy that must pass before the command is executed.
void Command::addPolicy(IPolicy* policy)
{
	_policies.push_back(policy);
}

// Check all policies against MSG, then run the command on SRV.
rfc Command::execute(IServerCtrl& srv, const Message& msg) const
{
	// TODO ensure POLICY and COMMAND errors are in line with PROTOCOL CODES
	for (size_t i = 0; i < _policies.size(); ++i)
	{
		rfc ret = _policies[i]->check(msg, srv);
		if (ret)
			return (ret);
	}
	return (_func(srv, msg));
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
