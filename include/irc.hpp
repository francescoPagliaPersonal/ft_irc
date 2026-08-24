/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   irc.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mweghofe <mweghofe@student.42vienna.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/24 17:48:24 by mweghofe          #+#    #+#             */
/*   Updated: 2026/08/24 18:20:15 by mweghofe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef IRC_HPP
# define IRC_HPP

# include <string>
# include <vector>

namespace irc
{
	void allCaps(std::string & str);
	bool isNameCompliant(const std::string& );
	std::vector<std::string> strSplit(std::string str, char ch, bool keepEmptyStr);

	enum rfc
	{
		// REPLY CODES
		OK = 0,
		WELCOME = 1,			// 001 RPL_WELCOME
		YOURHOST,				// 002 RPL_YOURHOST
		CREATED,				// 003 RPL_CREATED
		MYINFO,					// 004 RPL_MYINFO
		CHANNELMODEIS = 324,	//     RPL_CHANNELMODEIS
		NOTOPIC = 331,			//     RPL_NOTOPIC
		TOPIC,					// 332 RPL_TOPIC
		INVITING = 341,			//     RPL_INVITING
		NAMREPLY = 353,			//     RPL_NAMREPLY
		ENDOFNAMES = 366,		//     RPL_ENDOFNAMES
		MOTD = 372,				//     RPL_MOTD
		MOTDSTART = 375,		//     RPL_MOTDSTART 
		ENDOFMOTD,				// 376 RPL_ENDOFMOTD
		// ERROR CODES
		NOSUCHNICK = 401,		//     ERR_NOSUCHNICK
		NOSUCHCHANNEL = 403,	//     ERR_NOSUCHCHANNEL
		CANNOTSEND = 404,		//     ERR_CANNOTSENDTOCHAN
		TOOMANYCHANS,			// 405 ERR_TOOMANYCHANNELS
		INVALIDCAPCMD = 410,	//     ERR_INVALIDCAPCMD (IRCv3)
		NORECIPIENT,			//     ERR_NORECIPIENT
		NOTEXT,					// 412 ERR_NOTEXTTOSEND
		BADCMD = 421,			//     ERR_UNKNOWNCOMMAND
		NONICK = 431,			//     ERR_NONICKNAMEGIVEN
		NICKBAD,				// 432 ERR_ERRONEUSNICKNAME
		NICKINUSE,				// 433 ERR_NICKNAMEINUSE
		NOTINCHANNEL = 441,		//     ERR_USERNOTINCHANNEL
		NOTONCHANNEL,			// 442 ERR_NOTONCHANNEL
		USERONCHANNEL,			// 443 ERR_USERONCHANNEL
		NOTREG = 451,			//     ERR_NOTREGISTERED
		FEWPARAMS = 461,		//     ERR_NEEDMOREPARAMS
		ALREADYREG,				// 462 ERR_ALREADYREGISTERED
		BADPASS = 464,			//     ERR_PASSWDMISMATCH
		CHANFULL = 471,			//     ERR_CHANNELISFULL
		UNKNOWNMODE,			// 472 ERR_UNKNOWNMODE
		INVITEONLY,				// 473 ERR_INVITEONLYCHAN
		BADCHANNELKEY = 475,	//     ERR_BADCHANNELKEY
		BADCHANMASK,			//     ERR_BADCHANMASK
		CHANOPRIVS = 482,		//     ERR_CHANOPRIVSNEEDED
		NOCONN = 42001,			// internal: sender gone
		MANYPARAMS				// internal: too many parameters // TODO probably wrong place => parsing topic?
	};


} // end of namespace IRC

typedef irc::rfc rfc;

#endif
