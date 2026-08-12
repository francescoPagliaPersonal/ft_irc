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


static void allCaps(std::string & str) 
{
	for (std::string::size_type i = 0; i < str.size(); ++i)
		str[i] = std::toupper(static_cast<unsigned char>(str[i]));
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

Message string2Message(const std::string & raw, Client *client) 
{
	Message msg;
	msg.flags = 0;
	msg.sender = client;
	
	std::string str = raw;
	std::string::size_type start = str.find_first_not_of(' ');
	
	// Clear spaces at beggining
	// if the message is empty is returned as is and the flags will say so!
	if (start != std::string::npos) 
		str.erase(0, start);
	else
		return msg;
	
	// Extract prefix
	if (str[0] == ':') 
	{
		std::string::size_type end = str.find_first_of(' ');
		if (end != std::string::npos) 
		{
			msg.prefix = str.substr(1, end - 1);
			str.erase(0, end + 1);
		}
		else
		{
			msg.prefix = str.substr(1);
			msg.flags |= MSG_HAS_PREFIX;
			return msg;
		}
		msg.flags |= MSG_HAS_PREFIX;
	}
	
	// Extract trailing if the patter is " :" must have a space infront
	std::string::size_type trailing_pos = str.find_first_of(':');
	if (trailing_pos != std::string::npos && str[trailing_pos - 1] == ' ') 
	{
		msg.trailing = str.substr(trailing_pos + 1);
		str.erase(trailing_pos);
		msg.flags |= MSG_HAS_TRAILING;
		
		// Clear the remaining string of trailing and any additional space
		std::string::size_type last_non_space = str.find_last_not_of(' ');
		if (last_non_space != std::string::npos) 
			str.erase(last_non_space + 1);
	}
	
	// Extract command
	std::string::size_type pos = str.find_first_of(' ');
	if (pos != std::string::npos) 
	{
		msg.command = str.substr(0, pos);
		allCaps(msg.command);
		msg.flags |= MSG_HAS_COMMAND;
		str.erase(0, pos + 1);
	} 
	else if (!str.empty()) 
	{
		msg.command = str;
		allCaps(msg.command);
		msg.flags |= MSG_HAS_COMMAND;
		str.clear();
	}
	
	// Extract all params 
	while (!str.empty()) 
	{
		std::string::size_type start_pos = str.find_first_not_of(' ');
		if (start_pos == std::string::npos) 
			break;
		str.erase(0, start_pos);
		
		// Find end of param
		std::string::size_type end_pos = str.find_first_of(' ');
		if (end_pos != std::string::npos) 
		{
			msg.params.push_back(str.substr(0, end_pos));
			str.erase(0, end_pos);
		} 
		else 
		{
			msg.params.push_back(str);
			str.clear();
		}
	}
	
	if (!msg.params.empty())
		msg.flags |= MSG_HAS_PARAMS;
	
	return msg;
}
