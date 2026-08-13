/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parseMessage.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fpaglia <fpaglia@student.42vienna.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/12 09:01:01 by fpaglia           #+#    #+#             */
/*   Updated: 2026/08/12 09:01:04 by fpaglia          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Message.hpp"
#include <cctype>

namespace  {

	inline void clear_leading_char(std::string & str, unsigned char c)
	{
		std::string::size_type start = str.find_first_not_of(c);
		str.erase(0, start);
	}

	inline void allCaps(std::string & str) 
	{
		for (std::string::size_type i = 0; i < str.size(); ++i)
			str[i] = std::toupper(static_cast<unsigned char>(str[i]));
	}
}

int argCount(const Message & msg)
{
	size_t count = 0;
	count += (msg.flags & MSG_HAS_PREFIX) != 0 ;
	count += (msg.flags & MSG_HAS_COMMAND) != 0;
	count += (msg.flags & MSG_HAS_TRAILING) != 0;
	count += msg.params.size();
	return count;
}

Message string2Message(std::string str, Client *client) 
{
	Message msg;
	msg.flags = 0;
	msg.sender = client;
	
	// Clear spaces at beggining
	// if the message is empty is returned as is and the flags will say so!
	
	clear_leading_char(str, ' ');
	if (str.empty()) 
		return msg;		
	
	
	// Extract prefix
	if (str[0] == ':') 
	{
		std::string::size_type end = str.find_first_of(' ');
		if (end > 1) 
		{
			msg.prefix = str.substr(1, end);
			str.erase(0, end);
			msg.flags |= MSG_HAS_PREFIX;
		}
		if (str.empty()) 
			return msg;
	}
	
	// Extract trailing if the patter is " :" must have a space infront
	std::string::size_type trailing_pos = str.find(" :");
	if (trailing_pos != std::string::npos) 
	{
		msg.trailing = str.substr(trailing_pos + 2);
		str.erase(trailing_pos);
		msg.flags |= MSG_HAS_TRAILING;
		
		// Clear the remaining string of trailing and any additional space
		std::string::size_type last_non_space = str.find_last_not_of(' ');
		if (last_non_space != std::string::npos) 
			str.erase(last_non_space + 1);
	}

	clear_leading_char(str, ' ');
	
	// Extract command
	std::string::size_type pos = str.find_first_of(' ');
	if (pos)
	{
		msg.command = str.substr(0, pos);
		allCaps(msg.command);
		msg.flags |= MSG_HAS_COMMAND;
		str.erase(0, pos);
	}
	
	// Extract all params 
	while (!str.empty()) 
	{
		clear_leading_char(str, ' ');
		
		// Find end of param
		std::string::size_type end_pos = str.find_first_of(' ');
		if (end_pos > 0)
		{
			std::string tmp = str.substr(0, end_pos);
			msg.params.push_back(tmp);
			str.erase(0, end_pos);
		}
	}
	
	if (!msg.params.empty())
		msg.flags |= MSG_HAS_PARAMS;
	
	return msg;
}
