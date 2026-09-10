/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   irc.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mweghofe <mweghofe@student.42vienna.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/24 17:48:24 by mweghofe          #+#    #+#             */
/*   Updated: 2026/09/10 14:22:43 by mweghofe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef IRC_HPP
# define IRC_HPP

// -------------------------------------------------------------------------- //

# include <string>
# include <vector>

// -------------------------------------------------------------------------- //

namespace irc
{
	void allCaps(std::string & str);
	bool isNameCompliant(const std::string& );
	std::vector<std::string> strSplit(std::string str, char ch, bool keepEmptyStr);
	std::string chunkifyTrailing(const std::string & msgArgs, std::string msgTrailing);

	typedef unsigned int		uint;
	typedef unsigned char		uint8;
	typedef unsigned long int	uint32;
	
	enum epollret 
	{
		RET_OK,
		RET_EMPTY,
		RET_CLOSE,
		RET_HASOUTPUT,
		RET_PARSEINPUT
	};

	enum rfc
	{
		// REPLY CODES				--------------------------------------------
		OK = 0,						// internal: all good
		WELCOME,					// 001 RPL_WELCOME
		YOURHOST,					// 002 RPL_YOURHOST
		CREATED,					// 003 RPL_CREATED
		MYINFO,						// 004 RPL_MYINFO
		UMODEIS = 221,				// 221 RPL_UMODEIS
		AWAY = 301,					// 301 RPL_AWAY
		CHANNELMODEIS = 324,		// 324 RPL_CHANNELMODEIS
		CREATIONTIME = 329,			// 329 RPL_CREATIONTIME
		NOTOPIC = 331,				// 331 RPL_NOTOPIC
		TOPIC,						// 332 RPL_TOPIC
		TOPICWHOTIME,				// 333 RPL_TOPICWHOTIME
		INVITING = 341,				// 341 RPL_INVITING
		NAMREPLY = 353,				// 353 RPL_NAMREPLY
		ENDOFNAMES = 366,			// 366 RPL_ENDOFNAMES
		MOTD = 372,					// 372 RPL_MOTD
		MOTDSTART = 375,			// 375 RPL_MOTDSTART 
		ENDOFMOTD,					// 376 RPL_ENDOFMOTD
		YOUREOPER = 381,			// 381 RPL_YOUREOPER
		// ERROR CODES				--------------------------------------------
		NOSUCHNICK = 401,			// 401 ERR_NOSUCHNICK
		NOSUCHSERVER,				// 402 ERR_NOSUCHSERVER
		NOSUCHCHANNEL,				// 403 ERR_NOSUCHCHANNEL
		CANNOTSENDTOCHAN,			// 404 ERR_CANNOTSENDTOCHAN
		TOOMANYCHANNELS,			// 405 ERR_TOOMANYCHANNELS
		NOORIGIN = 409,				// 409 ERR_NOORIGIN
		INVALIDCAPCMD,				// 410 ERR_INVALIDCAPCMD (IRCv3)
		NORECIPIENT,				// 411 ERR_NORECIPIENT
		NOTEXTTOSEND,				// 412 ERR_NOTEXTTOSEND
		UNKNOWNCOMMAND = 421,		// 421 ERR_UNKNOWNCOMMAND
		NONICKNAMEGIVEN = 431,		// 431 ERR_NONICKNAMEGIVEN
		ERRONEUSNICKNAME,			// 432 ERR_ERRONEUSNICKNAME
		NICKNAMEINUSE,				// 433 ERR_NICKNAMEINUSE
		USERNOTINCHANNEL = 441,		// 441 ERR_USERNOTINCHANNEL
		NOTONCHANNEL,				// 442 ERR_NOTONCHANNEL
		USERONCHANNEL,				// 443 ERR_USERONCHANNEL
		NOTREGISTERED = 451,		// 451 ERR_NOTREGISTERED
		NEEDMOREPARAMS = 461,		// 461 ERR_NEEDMOREPARAMS
		KEYSET = 467,				// 467 ERR_KEYSET
		ALREADYREGISTERED,			// 462 ERR_ALREADYREGISTERED
		PASSWDMISMATCH = 464,		// 464 ERR_PASSWDMISMATCH
		CHANNELISFULL = 471,		// 471 ERR_CHANNELISFULL
		UNKNOWNMODE,				// 472 ERR_UNKNOWNMODE
		INVITEONLYCHAN,				// 473 ERR_INVITEONLYCHAN
		BANNEDFROMCHAN,				// 474 ERR_BANNEDFROMCHAN
		BADCHANNELKEY,				// 475 ERR_BADCHANNELKEY
		BADCHANMASK,				// 476 ERR_BADCHANMASK
		CHANOPRIVSNEEDED = 482,		// 482 ERR_CHANOPRIVSNEEDED
		NOOPERHOST = 491,			// 491 ERR_NOOPERHOST
		UMODEUNKNOWNFLAG = 501,		// 501 ERR_UMODEUNKNOWNFLAG
		USERSDONTMATCH,				// 502 ERR_USERSDONTMATCH
		MANYPARAMS = 42001,			// internal: too many parameters // TODO probably wrong place => parsing topic?
		HASQUIT						// internal: client send QUIT
	};

} // end of namespace IRC

typedef irc::rfc rfc;

#endif
