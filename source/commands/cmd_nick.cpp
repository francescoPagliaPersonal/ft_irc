/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_nick.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fpaglia <fpaglia@student.42vienna.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/12 13:54:12 by fpaglia           #+#    #+#             */
/*   Updated: 2026/08/26 14:27:51 by fpaglia          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


# include "Command.hpp"
# include "IServerCtrl.hpp"
# include "Client.hpp"
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
	
	client->setNick(newNick);

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
		// TODO we can most likely ditch this for only :newNick
		std::string response = Response::senderMessage(msg, newNick, 
												currNick + " has changed is nickname to " + newNick);
		srv.sendMessage(*client, response);
	}
	return (irc::OK);
}
