/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Message.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fpaglia <fpaglia@student.42vienna.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/12 09:01:01 by fpaglia           #+#    #+#             */
/*   Updated: 2026/08/20 11:25:21 by fpaglia          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Message.hpp"
#include "ft_irc.hpp"
#include <cctype>
#include <exception>
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


	inline void allCaps(std::string & str) 
	{
		for (std::string::size_type i = 0; i < str.size(); ++i)
			str[i] = std::toupper(static_cast<unsigned char>(str[i]));
	}
}

// Count the total number of parameters and trailing part of MSG.
int argCount(const Message & msg)
{
	size_t count = 0;
	// count += (msg.flags & MSG_HAS_PREFIX) != 0 ;
	// count += (msg.flags & MSG_HAS_COMMAND) != 0;
	count += (msg.flags & MSG_HAS_TRAILING) != 0;
	count += msg.params.size();
	return count;
}

// Parse STR into a Message struct for CLIENT, setting its flags accordingly.
Message string2Message(std::string str, Client *client) 
{
	Message msg;
	msg.flags = 0;
	msg.sender = client;
	
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
		clear_trailing_char(str, ' ');

		if (!msg.trailing.empty())
			msg.flags |= MSG_HAS_TRAILING;
	}

	clear_leading_char(str, ' ');
	
	if (str.empty())
		return msg;
	// Extract command
	std::string::size_type pos = str.find_first_of(' ');
	if (pos)
	{
		msg.command = str.substr(0, pos);
		allCaps(msg.command);
		str.erase(0, pos);

		msg.flags |= MSG_HAS_COMMAND;
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


std::vector<std::string> strSplit(std::string str, char ch, bool keepEmptyStr)
{
	std::vector<std::string>	words;

	std::string::size_type pos = str.find_first_of(ch);
	
	while (pos != std::string::npos)
	{
		if (pos == 0 && keepEmptyStr)
			words.push_back("");
		else
			words.push_back(str.substr(0,pos));
		str.erase(0,pos + 1);
		pos = str.find_first_of(ch);
	}
	if (str.size())
		words.push_back(str);
	
	return words;
}


std::vector<std::string> chunkyfyTrailing(size_t usedBuffer, std::string message)
{
	size_t availBuffer = usedBuffer < MSG_MAX_LENGTH - 4 ? MSG_MAX_LENGTH - usedBuffer - 4 : 0;
	if (!availBuffer)
		throw std::runtime_error("cannot build a message because the buffer used is too long.");
	if (message.size() > availBuffer)
	{
		/*TODO: find first \n
				if \n is not found or the lenght is still too long 
				then find closes space before [available buffer]
				if no spaces are found cut the message at available buffer 
				then repeat till message is empty.
		*/
		// if (message[availBuffer] != )
		
	}
	std::vector<std::string> tmp;
	tmp.push_back("");
	
	return tmp;
	// TODO: fill the topic section!!
}
