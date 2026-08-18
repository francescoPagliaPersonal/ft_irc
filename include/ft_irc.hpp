/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_irc.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mweghofe <mweghofe@student.42vienna.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/09 14:25:38 by mweghofe          #+#    #+#             */
/*   Updated: 2026/08/18 15:17:31 by mweghofe         ###   ########.fr       */
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
		NOSUCHNICK = 401,			//     ERR_NOSUCHNICK
		NOSUCHCHANNEL = 403,		//     ERR_NOSUCHCHANNEL
		CANNOTSENDTOCHAN = 404,		//     ERR_CANNOTSENDTOCHAN
		TOOMANYCHANNELS,			// 405 ERR_TOOMANYCHANNELS
		NORECIPIENT = 411,			//     ERR_NORECIPIENT
		NOTEXTTOSEND,				// 412 ERR_NOTEXTTOSEND
		UNKNOWNCOMMAND = 421,		//     ERR_UNKNOWNCOMMAND
		NONICKNAMEGIVEN = 431,		//     ERR_NONICKNAMEGIVEN
		ERRONEUSNICKNAME,			// 432 ERR_ERRONEUSNICKNAME
		NICKNAMEINUSE,				// 433 ERR_NICKNAMEINUSE
		USERNOTINCHANNEL = 441,		//     ERR_USERNOTINCHANNEL
		NOTONCHANNEL,				// 442 ERR_NOTONCHANNEL
		USERONCHANNEL,				// 443 ERR_USERONCHANNEL
		NOTREGISTERED = 451,		//     ERR_NOTREGISTERED
		NEEDMOREPARAMS = 461,		//     ERR_NEEDMOREPARAMS
		ALREADYREGISTERED,			// 462 ERR_ALREADYREGISTERED
		PASSWDMISMATCH = 464,		//     ERR_PASSWDMISMATCH
		CHANNELISFULL = 471,		//     ERR_CHANNELISFULL
		UNKNOWNMODE,				// 472 ERR_UNKNOWNMODE
		INVITEONLYCHAN,				// 473 ERR_INVITEONLYCHAN
		BADCHANNELKEY = 475,		//     ERR_BADCHANNELKEY
		CHANOPRIVSNEEDED = 482,		//     ERR_CHANOPRIVSNEEDED
		NOCONN = 42001,				// internal: sender gone
		// TODO probably wrong place => parsing topic?
		MANYPARAMS					// internal: too many parameters
	};

	enum e_rpl
	{
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
		ENDOFNAMES = 366		//     RPL_ENDOFNAMES
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
