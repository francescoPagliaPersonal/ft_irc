/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_nick.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mweghofe <mweghofe@student.42vienna.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/12 13:54:12 by fpaglia           #+#    #+#             */
/*   Updated: 2026/08/25 09:59:36 by mweghofe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


# include "Command.hpp"
# include "IServerCtrl.hpp"
# include "Client.hpp"
#include "Message.hpp"
#include "ft_irc.hpp"
#include "irc.hpp"
#include <string>


// Set the client's nick if it is not already taken by another client.
rfc cmd_nick(IServerCtrl & srv, const Message & msg)
{
	Client *client = msg.sender;
	std::string currNick = client->getNick();
	if (!(msg.flags & MSG_HAS_PARAMS))
		return (irc::NONICKNAMEGIVEN);
	
	std::string tmpNick = msg.params[0];
	if (!irc::isNameCompliant(tmpNick))
		return (irc::ERRONEUSNICKNAME);
	
	Client *hasThisNick = srv.findClientByNick(tmpNick);
	
	if (hasThisNick != NULL && hasThisNick != client)
	return (irc::NICKNAMEINUSE); // ERR_NICKCOLLISION
	
	std::string	response(":" + client->getID() + " " + msg.command);
	client->setNick(tmpNick);

	if (client->getRegistrationFlags() != REG_DONE)
	{
		client->setRegistrationFlags(REG_NICK);
		if (DEBUG == debug::DETAILED)
			std::cout << "[FD " << client->getFD()
				<< "] Nick registration successfull.\n";
		if (!client->getCap())
			srv.tryCompleteRegistration(*client);
	}
	else {
		response += " " + tmpNick + " :" + currNick + " has changed is nickname to " + tmpNick + CRLF;
		srv.sendMessage(*client, response);
	}
	return (irc::OK);
}
