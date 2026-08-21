/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   irc.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fpaglia <fpaglia@student.42vienna.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/21 11:27:30 by fpaglia           #+#    #+#             */
/*   Updated: 2026/08/21 11:28:25 by fpaglia          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_irc.hpp"

bool irc::isChannelCompliant(const std::string& channel)
{
	std::string validChar("-_");
	std::string mustNotStartWith("$:~&#@%+");
	
	if (channel.size() > MAX_CHANLEN)
		return false;
	for (size_t i = 1; i < channel.size(); ++i)
	{
		if (!std::isalnum(channel[i]) 
			|| validChar.find_first_of(channel[i]) == std::string::npos)
			return false;
	}
	return true;
}

bool irc::isNickCompliant(const std::string & nick)
{
	std::string mustNotContain(" .,*?!@");
	std::string mustNotStartWith("$:~&#@%+");
	
	if (nick.size() > MAX_NICKLEN)
		return false;
	if (!(nick.find_first_of(mustNotContain) == std::string::npos))
		return false;
	if (mustNotStartWith.find_first_of(nick[0]) != std::string::npos)
		return false;
	return true;
}
