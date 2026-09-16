/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Message.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mweghofe <mweghofe@student.42vienna.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/12 09:01:01 by fpaglia           #+#    #+#             */
/*   Updated: 2026/09/16 12:34:47 by mweghofe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Message.hpp"
#include "ft_irc.hpp"
#include "irc.hpp"
#include <cctype>
#include <cstddef>
#include <stdexcept>

namespace  {

	inline void clear_leading_char(std::string & str, unsigned char c)
	{
		std::string::size_type start = str.find_first_not_of(c);
		str.erase(0, start);
	}

	inline void clear_trailing_char(std::string & str, unsigned char c)
	{
		std::string::size_type start = str.find_last_not_of(c);
		str.erase(start + 1);
	}

}

irc::uint	Message::argCount() const
{
	return params.size();
}

std::string	Message::getTrailing() const
{
	if (flags & irc::MSG_HAS_TRAILING)
		return params.back();
	return "";
}

// Parse STR into a Message struct for CLIENT, setting its flags accordingly.
Message irc::string2Message(std::string str, Client *client) 
{
	Message msg;
	msg.flags = 0;
	msg.sender = client;

	std::string tmpTrail;
	
	// Clear spaces at beggining
	// if the message is empty is returned as is and the flags will say so!
	
	clear_leading_char(str, ' ');
	clear_trailing_char(str, ' ');
	if (str.empty()) 
		return msg;		
	
	
	// Extract prefix
	if (str[0] == ':') 
	{
		std::string::size_type end = str.find_first_of(' ');
		if (end > 1) 
		{
			msg.prefix = str.substr(1, end - 1);
			if (end == std::string::npos)
				clear_trailing_char(msg.prefix, ' ');
			str.erase(0, end);
			msg.flags |= irc::MSG_HAS_PREFIX;
		}
		if (str.empty())
		{
			msg.flags = 0;
			return msg;
		}
	}
	
	// Extract trailing if the patter is " :" must have a space infront
	std::string::size_type trailing_pos = str.find(" :");
	if (trailing_pos != std::string::npos) 
	{
		tmpTrail = str.substr(trailing_pos + 2);
		str.erase(trailing_pos);
		clear_trailing_char(str, ' ');

		msg.flags |= irc::MSG_HAS_TRAILING;
	}

	clear_leading_char(str, ' ');
	
	if (str.empty())
	{
		msg.flags = 0;
		return msg;
	}
	// Extract command
	std::string::size_type pos = str.find_first_of(' ');
	if (pos)
	{
		msg.command = str.substr(0, pos);
		allCaps(msg.command);
		str.erase(0, pos);

		msg.flags |= irc::MSG_HAS_COMMAND;
	}
	
	// Extract all params 
	while (!str.empty()) 
	{
		clear_leading_char(str, ' ');	
		// Find end of param; always > 0, npos just gives us the whole string
		std::string::size_type end_pos = str.find_first_of(' ');
		msg.params.push_back(str.substr(0, end_pos));
		str.erase(0, end_pos);
	}

	if (msg.flags & irc::MSG_HAS_TRAILING)
		msg.params.push_back(tmpTrail);

	if (!msg.params.empty())
		msg.flags |= irc::MSG_HAS_PARAMS;
	
	return msg;
}
