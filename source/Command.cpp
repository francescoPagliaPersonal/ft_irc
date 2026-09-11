/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Command.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mweghofe <mweghofe@student.42vienna.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/09 18:07:43 by mweghofe          #+#    #+#             */
/*   Updated: 2026/09/11 17:44:09 by mweghofe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Command.hpp"
#include "IPolicy.hpp"
#include "irc.hpp"

// -------------------------------------------------------------------------- //
// CUSTOM CTOR & DTOR
// -------------------------------------------------------------------------- //

#ifndef BONUS
// Custom constructor to create a command with NAME and its handler FUNC.
Command::Command(const std::string& name, command func)
	: _name(name)
	, _policies()
	, _func(func)
{}
# else
// Custom construtor to create a command for the bot.
Command::Command(const std::string& name, botcmd func)
	: _name(name)
	, _policies()
	, _func(func)
{}

#endif

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

#ifdef BONUS

// Execute the called bot command.
void Command::execute(IBot& bot, const Message &msg, std::vector<std::string> botcmds) const
{
	_func(bot, msg, botcmds);
}

# else

// Check all policies against MSG, then run the command on SRV.
rfc Command::execute(IServerCtrl& srv, const Message& msg) const
{
	// TODO ensure POLICY and COMMAND errors are in line with PROTOCOL CODES
	for (size_t i = 0; i < _policies.size(); ++i)
	{
		rfc ret = _policies[i]->check(msg);
		if (ret)
			return (ret);
	}
	return (_func(srv, msg));
}

#endif

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
