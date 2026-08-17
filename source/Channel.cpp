/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mweghofe <mweghofe@student.42vienna.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/13 10:59:59 by mweghofe          #+#    #+#             */
/*   Updated: 2026/08/17 17:41:46 by mweghofe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Channel.hpp"

// -------------------------------------------------------------------------- //
// CUSTOM CTOR & DTOR
// -------------------------------------------------------------------------- //

// Custom constructor to create a channel with TITLE and password PW.
Channel::Channel(const std::string& title, const std::string& pw)
	: _modes(0)
	, _userLimit(MAX_CHANNELUSERS)
	, _title(title)
	, _topic()
	, _password(pw)
	, _members()
{}

// Channel destructor.
Channel::~Channel()
{}

// -------------------------------------------------------------------------- //
// OCF - only declared, not defined, unusable
// -------------------------------------------------------------------------- //
