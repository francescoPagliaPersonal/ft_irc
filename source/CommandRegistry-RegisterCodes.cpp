/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CommandRegistry-RegisterCodes.cpp                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mweghofe <mweghofe@student.42vienna.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/18 12:35:10 by mweghofe          #+#    #+#             */
/*   Updated: 2026/08/20 09:15:58 by mweghofe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "CommandRegistry.hpp"
#include "RFCnumeric.hpp"
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

std::string nickInUse(const Message& msg)
{
	std::string rpl;
	rpl = msg.params[0] + " :Nickname is already in use." + CRLF;
	return (rpl);
}

std::string badCmd(const Message& msg)
{
	std::string rpl;
	rpl = "<" + msg.command + "> :Command not found." + CRLF;
	return (rpl);
}

std::string notRegistered(const Message& msg)
{
	std::string rpl(":You have not registered.");
	(void) msg;
	rpl += CRLF;
	return (rpl);
}

std::string tooFewParams(const Message& msg)
{
	std::string rpl;
	rpl = '<' + msg.command + "> :Not enough parameters." + CRLF;
	return (rpl);
}

std::string alreadyRegistered(const Message& msg)
{
	std::string rpl(":This user is already registered.");
	(void) msg;
	rpl += CRLF;
	return (rpl);
}

std::string badPassword(const Message& msg)
{
	std::string rpl(":Password incorrect.");
	(void) msg;
	rpl += CRLF;
	return (rpl);
}

std::string noNick(const Message& msg)
{
	std::string rpl(":No nickname given.");
	(void) msg;
	rpl += CRLF;
	return (rpl);
}

std::string nickBad(const Message& msg)
{
	std::string rpl;
	rpl = msg.params[0] + " :Erroneous Nickname." + CRLF;
	return (rpl);
}

}
