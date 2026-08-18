/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CommandRegistry-RegisterCodes.cpp                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mweghofe <mweghofe@student.42vienna.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/18 12:35:10 by mweghofe          #+#    #+#             */
/*   Updated: 2026/08/18 15:05:27 by mweghofe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "CommandRegistry.hpp"
#include <sstream>

void CommandRegistry::registerCodes()
{
	_rfcCodes[rfc::BADCMD] = rfc::badCmd;
	_rfcCodes[rfc::NICKINUSE] = rfc::nickInUse;
	_rfcCodes[rfc::NOTREG] = rfc::notRegistered;
	_rfcCodes[rfc::ALREADYREG] = rfc::alreadyRegistered;
	_rfcCodes[rfc::FEWPARAMS] = rfc::tooFewParams;
	_rfcCodes[rfc::BADPASS] = rfc::badPassword;
	_rfcCodes[rfc::NONICK] = rfc::noNick;
	_rfcCodes[rfc::NICKBAD] = rfc::nickBad;
}

namespace rfc
{

std::string nickInUse(const Message& msg, const std::string& nick)
{
	std::stringstream ss;
	ss << rfc::NICKINUSE << ' ' << nick << ' ' << msg.params[0]
		<< " :Nickname is already in use." << CRLF;
	return (ss.str());
}

std::string badCmd(const Message& msg, const std::string& nick)
{
	std::stringstream ss;
	ss << rfc::BADCMD << ' ' << nick << " <" << msg.command
		<< "> :Command not found." << CRLF;
	return (ss.str());
}

std::string notRegistered(const Message& msg, const std::string& nick)
{
	std::stringstream ss;
	(void) msg;
	ss << rfc::NOTREG << ' ' << nick
		<< " :You have not registered." << CRLF;
	return (ss.str());
}

std::string tooFewParams(const Message& msg, const std::string& nick)
{
	std::stringstream ss;
	ss << rfc::FEWPARAMS << ' ' << nick << " <" << msg.command
		<< "> :Not enough parameters." << CRLF;
	return (ss.str());
}

std::string alreadyRegistered(const Message& msg, const std::string& nick)
{
	std::stringstream ss;
	(void) msg;
	ss << rfc::ALREADYREG << ' ' << nick
		<< " :This user is already registered." << CRLF;
	return (ss.str());
}

std::string badPassword(const Message& msg, const std::string& nick)
{
	std::stringstream ss;
	(void) msg;
	ss << rfc::BADPASS << ' ' << nick
		<< " :Password incorrect." << CRLF;
	return (ss.str());
}

std::string noNick(const Message& msg, const std::string& nick)
{
	std::stringstream ss;
	(void) msg;
	ss << rfc::NONICK << ' ' << nick
		<< " :No nickname given." << CRLF;
	return (ss.str());
}

std::string nickBad(const Message& msg, const std::string& nick)
{
	std::stringstream ss;
	ss << rfc::NICKBAD << ' ' << nick << ' ' << msg.params[0]
		<< " :Erroneous Nickname." << CRLF;
	return (ss.str());
}

}
