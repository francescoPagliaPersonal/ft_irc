/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RFCnumeric.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mweghofe <mweghofe@student.42vienna.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/20 09:23:54 by mweghofe          #+#    #+#             */
/*   Updated: 2026/08/20 11:19:41 by mweghofe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "RFCnumeric.hpp"
#include "Message.hpp"

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
