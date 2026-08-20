/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_nick.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mweghofe <mweghofe@student.42vienna.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/12 13:54:12 by fpaglia           #+#    #+#             */
/*   Updated: 2026/08/20 09:20:16 by mweghofe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "Command.hpp"
#include "IServerCtrl.hpp"
#include "Client.hpp"
#include "Message.hpp"
#include "RFCnumeric.hpp"
#include <string>

namespace {
	bool isCompliant(std::string& nick)
	{
		std::string mustNotContain(" .,*?!@");
		std::string mustNotStartWith("$:~&#@%+");
		
		if (nick.size() > MAX_NICKLEN)
			return false;
		if (!(nick.find_first_of(mustNotContain) == std::string::npos))
			return false;
		if (mustNotStartWith.find_first_of(nick[0]) != std::string::npos)
			return false;
		return true;
	}
}

// Set the client's nick if it is not already taken by another client.
int cmd_nick(IServerCtrl & srv, const Message & msg)
{
	Client *client = msg.sender;
	std::string currNick = client->getNick();
	if (!(msg.flags & MSG_HAS_PARAMS))
		return (rfc::NONICKNAMEGIVEN);
	std::string tmpNick = msg.params[0];
	Client *hasThisNick = srv.findClientByNick(tmpNick);
	std::string	response(":" + currNick + " " + msg.command);
	
	if (hasThisNick != NULL && hasThisNick != client)
		return (rfc::NICKNAMEINUSE); // ERR_NICKCOLLISION
	if (!isCompliant(tmpNick))
		return (rfc::ERRONEUSNICKNAME);
	client->setNick(tmpNick);

	if (client->getRegistrationFlags() != REG_DONE)
	{
		client->setRegistrationFlags(REG_NICK);
		if (DEBUG == debug::DETAILED)
			std::cout << "[FD " << client->getFD()
				<< "] Nick registration successfull.\n";
		srv.tryCompleteRegistration(*client);
	}
	else {
		response += " " + tmpNick + " :" + currNick + " has changed is nickname to " + tmpNick + CRLF;
		srv.sendMessage(*client, response);
	}
	return (rfc::OK);
}
