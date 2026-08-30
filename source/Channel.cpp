/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mweghofe <mweghofe@student.42vienna.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/13 10:59:59 by mweghofe          #+#    #+#             */
/*   Updated: 2026/08/30 11:56:00 by mweghofe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Channel.hpp"

// -------------------------------------------------------------------------- //
// CUSTOM CTOR & DTOR
// -------------------------------------------------------------------------- //

// Custom constructor to create a channel with TITLE and password PW.
Channel::Channel(const std::string& title, const std::string& pw)
	: _modes(pw.empty() ? 0 : CH_PASSWORD)
	, _userLimit(0) // TODO needs proper initialization
	, _title(title)
	, _topic("Welcome to this beautiful channel!") // TODO use a macro
	, _password(pw)
	, _members()
{}

// Channel destructor.
Channel::~Channel()
{
	if (!_members.empty())
		_members.clear();
	if (!_invites.empty())
		_invites.clear();
}

// -------------------------------------------------------------------------- //
// OCF - only declared, not defined, unusable
// -------------------------------------------------------------------------- //
