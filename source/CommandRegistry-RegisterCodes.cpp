/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CommandRegistry-RegisterCodes.cpp                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mweghofe <mweghofe@student.42vienna.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/18 12:35:10 by mweghofe          #+#    #+#             */
/*   Updated: 2026/08/18 15:18:23 by mweghofe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "CommandRegistry.hpp"
#include <sstream>

void CommandRegistry::registerCodes()
{
	_rfcCodes[rfc::UNKNOWNCOMMAND] = rfc::badCmd;
	_rfcCodes[rfc::NICKNAMEINUSE] = rfc::nickInUse;
	_rfcCodes[rfc::NOTREGISTERED] = rfc::notRegistered;
	_rfcCodes[rfc::ALREADYREGISTERED] = rfc::alreadyRegistered;
	_rfcCodes[rfc::NEEDMOREPARAMS] = rfc::tooFewParams;
	_rfcCodes[rfc::PASSWDMISMATCH] = rfc::badPassword;
	_rfcCodes[rfc::NONICKNAMEGIVEN] = rfc::noNick;
	_rfcCodes[rfc::ERRONEUSNICKNAME] = rfc::nickBad;
}

namespace rfc
{

std::string nickInUse(const Message& msg, const std::string& nick)
{
	std::stringstream ss;
	ss << rfc::NICKNAMEINUSE << ' ' << nick << ' ' << msg.params[0]
		<< " :Nickname is already in use." << CRLF;
	return (ss.str());
}

std::string badCmd(const Message& msg, const std::string& nick)
{
	std::stringstream ss;
	ss << rfc::UNKNOWNCOMMAND << ' ' << nick << " <" << msg.command
		<< "> :Command not found." << CRLF;
	return (ss.str());
}

std::string notRegistered(const Message& msg, const std::string& nick)
{
	std::stringstream ss;
	(void) msg;
	ss << rfc::NOTREGISTERED << ' ' << nick
		<< " :You have not registered." << CRLF;
	return (ss.str());
}

std::string tooFewParams(const Message& msg, const std::string& nick)
{
	std::stringstream ss;
	ss << rfc::NEEDMOREPARAMS << ' ' << nick << " <" << msg.command
		<< "> :Not enough parameters." << CRLF;
	return (ss.str());
}

std::string alreadyRegistered(const Message& msg, const std::string& nick)
{
	std::stringstream ss;
	(void) msg;
	ss << rfc::ALREADYREGISTERED << ' ' << nick
		<< " :This user is already registered." << CRLF;
	return (ss.str());
}

std::string badPassword(const Message& msg, const std::string& nick)
{
	std::stringstream ss;
	(void) msg;
	ss << rfc::PASSWDMISMATCH << ' ' << nick
		<< " :Password incorrect." << CRLF;
	return (ss.str());
}

std::string noNick(const Message& msg, const std::string& nick)
{
	std::stringstream ss;
	(void) msg;
	ss << rfc::NONICKNAMEGIVEN << ' ' << nick
		<< " :No nickname given." << CRLF;
	return (ss.str());
}

std::string nickBad(const Message& msg, const std::string& nick)
{
	std::stringstream ss;
	ss << rfc::ERRONEUSNICKNAME << ' ' << nick << ' ' << msg.params[0]
		<< " :Erroneous Nickname." << CRLF;
	return (ss.str());
}

}
