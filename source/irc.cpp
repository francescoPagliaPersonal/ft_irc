/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   irc.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mweghofe <mweghofe@student.42vienna.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/21 11:27:30 by fpaglia           #+#    #+#             */
/*   Updated: 2026/09/11 13:16:31 by mweghofe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_irc.hpp"
#include "irc.hpp"

#include <ctime>

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

/* Given a message of any lenght that includes the complete formatting, split the trailing apart
 in multiple chuncks and return a string that contains as many message as needed that fit in MSG_MAX_LENGTH
 given this example:
 	:dan!~h@localhost PRIVMSG #coolpeople :the message
 the lenght of each part is
 nick = 32
 user = 32
 host = between 64 and 254
 chat = 32
 max total of the msgArgs is 1 + 32 + 1 + 32 + 1 + 254 + 1 + 7 +1 32 + 2 = 364
 by setting the MIN_TRAIL_LENGTH to 32 should be more than safe
if \n is not found or the lenght is still too long 
then find closes space before [max trail len]
if no spaces are found cut the message at available buffer 
then repeat till message is empty.
*/
std::string irc::chunkifyTrailing(const std::string & msgArgs, std::string msgTrailing)
{
	size_t eomsg = 2; //\CRLF
	std::string response;

	if (msgArgs.size() + MIN_TRAIL_LENGTH + eomsg > MSG_MAX_LENGTH )
		return std::string(""); //TODO: verify if this could even occur and if we should send a numeric

	size_t maxTrailLen = MSG_MAX_LENGTH - (msgArgs.size() + eomsg);

	while (msgTrailing.size() + eomsg > maxTrailLen)
	{
		std::string::size_type pos;
		pos = msgTrailing.find_last_of('\n', maxTrailLen);
		if (pos == std::string::npos)
			pos = msgTrailing.find_last_of(".;,: \t", maxTrailLen);
		if (pos == std::string::npos)
			pos = maxTrailLen;
		response += msgArgs + msgTrailing.substr(0, pos) + CRLF;
		msgTrailing.erase(0, pos);
		
	}
	response += msgArgs + msgTrailing + CRLF;

	return response;
}

std::string irc::timeNowStr()
{
	std::time_t now = std::time(NULL);
	char buffer[9];
	std::strftime(buffer, sizeof(buffer), "%H:%M:%S", std::localtime(&now));
	return (std::string (buffer));
}

std::string irc::timeAsStr(std::time_t aTime)
{
	char buffer[9];
	std::strftime(buffer, sizeof(buffer), "%H:%M:%S", std::localtime(&aTime));
	return (std::string (buffer));
}

