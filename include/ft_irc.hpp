/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_irc.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mweghofe <mweghofe@student.42vienna.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/09 14:25:38 by mweghofe          #+#    #+#             */
/*   Updated: 2026/08/19 13:26:31 by mweghofe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_IRC_HPP
# define FT_IRC_HPP

// -------------------------------------------------------------------------- //

# include <iostream>
# include <stdexcept>

// -------------------------------------------------------------------------- //

# define MAX_CLIENTS 50000
# define MAX_EVENTS 32
# define TIMEOUT 1000
# define CRLF "\r\n"
# define MSG_MAX_LENGTH 512
# define MAX_NICKLEN 32

# ifndef DEBUG
#  define DEBUG 0 // TODO set this to zero later and ctl via makefile
# endif

// -------------------------------------------------------------------------- //

namespace debug
{
	enum e_debug
	{
		LOGSONLY = 0,
		BASIC,
		DETAILED
	};
}

// -------------------------------------------------------------------------- //

namespace rfc
{
	enum e_err
	{
		NOSUCHNICK = 401,		// ERR_NOSUCHNICK
		NOSUCHCHANNEL = 403,		// ERR_NOSUCHCHANNEL
		CANNOTSEND = 404,		// ERR_CANNOTSENDTOCHAN
		TOOMANYCHANS,			// 405 ERR_TOOMANYCHANNELS
		NORECIPIENT = 411,		// ERR_NORECIPIENT
		NOTEXT,					// 412 ERR_NOTEXTTOSEND
		BADCMD = 421,			// ERR_UNKNOWNCOMMAND
		NONICK = 431,			// ERR_NONICKNAMEGIVEN
		NICKBAD,				// 432 ERR_ERRONEUSNICKNAME
		NICKINUSE,				// 433 ERR_NICKNAMEINUSE
		NOTINCHANNEL = 441,		// ERR_USERNOTINCHANNEL
		NOTONCHANNEL,			// 442 ERR_NOTONCHANNEL
		ONCHANNEL,				// 443 ERR_USERONCHANNEL
		NOTREG = 451,			// ERR_NOTREGISTERED
		FEWPARAMS = 461,		// ERR_NEEDMOREPARAMS
		ALREADYREG,				// 462 ERR_ALREADYREGISTERED
		BADPASS = 464,			// ERR_PASSWDMISMATCH
		CHANFULL = 471,			// ERR_CHANNELISFULL
		UNKNOWNMODE,			// 472 ERR_UNKNOWNMODE
		INVITEONLY,				// 473 ERR_INVITEONLYCHAN
		BADKEY = 475,			// ERR_BADCHANNELKEY
		CHANOPRIVS = 482,		// ERR_CHANOPRIVSNEEDED
		NOCONN = 42001,			// internal: sender gone
		MANYPARAMS				// internal: too many parameters // TODO probably wrong place => parsing topic?
	};

	enum e_rpl
	{
		OK = 0,
		WELCOME = 1,			// 001 RPL_WELCOME
		YOURHOST,				// 002 RPL_YOURHOST
		CREATED,				// 003 RPL_CREATED
		MYINFO,					// 004 RPL_MYINFO
		CHANNELMODEIS = 324,	// RPL_CHANNELMODEIS
		NOTOPIC = 331,			// RPL_NOTOPIC
		TOPIC,					// 332 RPL_TOPIC
		INVITING = 341,			// RPL_INVITING
		NAMREPLY = 353,			// RPL_NAMREPLY
		ENDOFNAMES = 366,		// RPL_ENDOFNAMES
		MOTD = 372,				//      RPL_MOTD
		MOTDSTART = 375,		//      RPL_MOTDSTART 
		ENDOFMOTD				// 376  RPL_ENDOFMOTD
	};
} // end namespace rfc

// -------------------------------------------------------------------------- //

enum e_pollret 
{
	RET_OK,
	RET_EMPTY,
	RET_CLOSE,
	RET_HASOUTPUT,
	RET_PARSEINPUT
};

// -------------------------------------------------------------------------- //

typedef unsigned int		t_uint;
typedef unsigned char		t_uint8;
typedef unsigned long int	t_uint32;

#endif
