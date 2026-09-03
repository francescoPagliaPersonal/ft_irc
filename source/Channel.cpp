/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fpaglia <fpaglia@student.42vienna.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/13 10:59:59 by mweghofe          #+#    #+#             */
/*   Updated: 2026/09/02 14:21:13 by fpaglia          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Channel.hpp"
#include "ft_irc.hpp"

// -------------------------------------------------------------------------- //
// CUSTOM CTOR & DTOR
// -------------------------------------------------------------------------- //

// Custom constructor to create a channel with TITLE and password PW.
Channel::Channel(const std::string& title, const std::string& pw)
	: _modes(pw.empty() ? 0 : CH_PASSWORD)
	, _userLimit(MAX_CHANNELUSERS)
	, _chanOps(0)
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
