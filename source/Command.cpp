/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Command.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mweghofe <mweghofe@student.42vienna.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/09 18:07:43 by mweghofe          #+#    #+#             */
/*   Updated: 2026/08/16 15:04:00 by mweghofe         ###   ########.fr       */
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
{
	// TODO fill me up
}

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
	Client *client = msg.sender;
	if (client == NULL)
		return 1; // ERR_HANGUP??

	std::cout 
		<< "Commad::" << __FUNCTION__ << "->" << _name
		<<std::endl;
	
	for (size_t i = 0; i < _policies.size(); ++i)
	{
		int ret = _policies[i]->check(msg, srv);
		// TODO the policy returns might need to be properly interpreted
		if (ret)
			return (ret);
	}
	std::cout 
		<< "Command::" << __FUNCTION__ << "->" << _name
		<< " Passed policy check..." << std::endl;

	// TODO ensure commands have a PROTOCOL ERROR CODE if they need to return one
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
