/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_nick.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mweghofe <mweghofe@student.42vienna.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/12 13:54:12 by fpaglia           #+#    #+#             */
/*   Updated: 2026/08/27 11:07:17 by mweghofe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "Command.hpp"
#include "IServerCtrl.hpp"
#include "Client.hpp"
#include "Message.hpp"
#include "Response.hpp"
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
	
	std::string newNick = msg.params[0];
	if (!irc::isNameCompliant(newNick))
		return (irc::ERRONEUSNICKNAME);
	
	Client *hasThisNick = srv.findClientByNick(newNick);
	
	if (hasThisNick != NULL && hasThisNick != client)
		return (irc::NICKNAMEINUSE); // ERR_NICKCOLLISION

	// user is still in registration process
	if (client->getRegistrationFlags() != REG_DONE)
	{
		client->setNick(newNick);
		client->setRegistrationFlags(REG_NICK);
		if (DEBUG == debug::DETAILED)
			std::cout << "[FD " << client->getFD()
				<< "] Nick registration successfull.\n";
		if (!client->getCap())
			srv.tryCompleteRegistration(*client);
	}
	// regular change of nick
	else {
		// TODO we can most likely ditch the trail for only :newNick
		std::string response = Response::buildRegular(
			msg, newNick, currNick + " has changed is nickname to " + newNick);
		client->setNick(newNick);
		srv.sendMessage(*client, response);
	}
	return (irc::OK);
}
