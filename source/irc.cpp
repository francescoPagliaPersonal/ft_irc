/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   irc.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mweghofe <mweghofe@student.42vienna.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/21 11:27:30 by fpaglia           #+#    #+#             */
/*   Updated: 2026/08/24 18:21:02 by mweghofe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_irc.hpp"
#include "irc.hpp"

void irc::allCaps(std::string & str) 
{
	for (std::string::size_type i = 0; i < str.size(); ++i)
		str[i] = std::toupper(static_cast<unsigned char>(str[i]));
}

// Validates NICK and channel PASSWORD.
bool irc::isNameCompliant(const std::string & word)
{
	std::string mustNotContain(" .,*?!@");
	std::string mustNotStartWith("$:~&#@%+");
	
	if (word.size() > MAX_NICKLEN)
		return false;
	if (!(word.find_first_of(mustNotContain) == std::string::npos))
		return false;
	if (mustNotStartWith.find_first_of(word[0]) != std::string::npos)
		return false;
	return true;
}

std::vector<std::string> irc::strSplit(std::string str, char ch, bool keepEmptyStr)
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
