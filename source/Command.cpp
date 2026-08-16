/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Command.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mweghofe <mweghofe@student.42vienna.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/09 18:07:43 by mweghofe          #+#    #+#             */
/*   Updated: 2026/08/16 19:10:15 by mweghofe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Command.hpp"
#include "IPolicy.hpp"

// -------------------------------------------------------------------------- //
// CUSTOM CTOR & DTOR
// -------------------------------------------------------------------------- //

Command::Command(const std::string& name, command func)
	: _name(name)
	, _func(func)
	, _policies()
{}

Command::~Command()
{
	std::vector<IPolicy*>::size_type i;
	for (i = 0; i < _policies.size(); ++i)
	{
		delete _policies[i];
	}
}

// -------------------------------------------------------------------------- //
// OPERATION
// -------------------------------------------------------------------------- //

std::string Command::getName() const
{
	return (_name);
}

void Command::addPolicy(IPolicy* policy)
{
	_policies.push_back(policy);
}

int Command::execute(IServerCtrl& srv, const Message& msg) const
{
	// TODO ensure POLICY and COMMAND errors are in line with PROTOCOL CODES
	for (size_t i = 0; i < _policies.size(); ++i)
	{
		int ret = _policies[i]->check(msg, srv);
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
