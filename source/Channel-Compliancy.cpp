/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel-Compliancy.cpp                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fpaglia <fpaglia@student.42vienna.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/21 12:12:19 by fpaglia           #+#    #+#             */
/*   Updated: 2026/08/21 15:43:28 by fpaglia          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Channel.hpp"
#include <algorithm>
#include <deque>
#include <map>
#include <ratio>

bool Channel::isTitleCompliant(const std::string& channel)
{
	std::string validChar("-_");
	
	if (channel.size() > MAX_CHANLEN)
		return false;
	if (channel[0] != '#' || channel[0] != '&' )
		return false;
	for (size_t i = 1; i < channel.size(); ++i)
	{
		if (!std::isalnum(channel[i]) 
			|| validChar.find_first_of(channel[i]) == std::string::npos)
			return false;
	}
	return true;
}

bool Channel::isMember(Client* client)
{
	std::map<Client *, bitMask>::iterator it;
	
	it = _members.find(client);
	return (it == _members.end() ? false : true);
}

bool Channel::passwordMatch(const std::string & pw)
{
	if (_password == pw)
		return true;
	// this option is to match irssi that return an x in case someone 
	// prompted an empty password in a list ch1,ch2 ,bla .
	return (_password.empty() && (pw == "x" || pw.empty()));
}

bool Channel::belowChannelLimit()
{
	return (!(_userLimit && (_members.size() < _userLimit)));
};

bool Channel::joinGranted(Client *client)
{
	std::deque<Client*>::iterator it; ;
	
	if (!(_modes & CH_INVITE))
		return true;
	
	for (it = _invites.begin(); it != _invites.end(); ++it)
	{
		if (*it == client)
			return true;
	}
	return false;
}
