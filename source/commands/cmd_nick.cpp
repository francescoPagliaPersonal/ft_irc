/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_nick.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fpaglia <fpaglia@student.42vienna.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/12 13:54:12 by fpaglia           #+#    #+#             */
/*   Updated: 2026/08/21 11:17:38 by fpaglia          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


# include "Command.hpp"
# include "IServerCtrl.hpp"
# include "Client.hpp"
#include "Message.hpp"
#include "ft_irc.hpp"
#include <string>


// Set the client's nick if it is not already taken by another client.
int cmd_nick(IServerCtrl & srv, const Message & msg)
{
	Client *client = msg.sender;
	std::string currNick = client->getNick();
	if (!(msg.flags & MSG_HAS_PARAMS))
		return (rfc::NONICK);
	
	std::string tmpNick = msg.params[0];
	if (!irc::isNickCompliant(tmpNick))
		return (rfc::NICKBAD);
	
	Client *hasThisNick = srv.findClientByNick(tmpNick);
	std::string	response(":" + currNick + " " + msg.command);
	
	if (hasThisNick != NULL && hasThisNick != client)
		return (rfc::NICKINUSE); // ERR_NICKCOLLISION
	
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
	return (rfc::OK);
}
