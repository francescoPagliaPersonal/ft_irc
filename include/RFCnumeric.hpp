/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RFCnumeric.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mweghofe <mweghofe@student.42vienna.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/20 09:12:05 by mweghofe          #+#    #+#             */
/*   Updated: 2026/08/20 09:17:40 by mweghofe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef RFCNUMERIC_HPP
# define RFCNUMEIC_HPP

// -------------------------------------------------------------------------- //

#include <string>

struct Message;

namespace rfc
{

// -------------------------------------------------------------------------- //

std::string badCmd(const Message&);
std::string nickInUse(const Message&);
std::string notRegistered(const Message&);
std::string alreadyRegistered(const Message&);
std::string tooFewParams(const Message&);
std::string badPassword(const Message&);
std::string noNick(const Message&);
std::string nickBad(const Message&);

// -------------------------------------------------------------------------- //

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
	// TODO probably wrong place => parsing topic?
	MANYPARAMS = 42001			// internal: too many parameters
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
	ENDOFNAMES = 366,		//     RPL_ENDOFNAMES
	MOTD = 372,				//     RPL_MOTD
	MOTDSTART = 375,		//     RPL_MOTDSTART 
	ENDOFMOTD				// 376 RPL_ENDOFMOTD
};

// -------------------------------------------------------------------------- //

} // end of namespace

#endif
