/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_nick.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mweghofe <mweghofe@student.42vienna.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/12 13:54:12 by fpaglia           #+#    #+#             */
/*   Updated: 2026/08/16 19:05:58 by mweghofe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


# include "Command.hpp"
# include "IServerCtrl.hpp"
# include "Client.hpp"

int cmd_nick(IServerCtrl & srv, const Message & msg)
{
	// TODO: this is just a quick proof of concept more complex nick evaluation 
	// should be carried out (against the whole server).
	// TODO also need charset validation?

	Client *client = msg.sender;
	if (client == NULL)
		return (rfc::NOCONN); // ERR_HANGUP??
	std::string tmpNick = msg.params[0];
	Client *hasThisNick = srv.findClientByNick(tmpNick);
	if (hasThisNick != NULL && hasThisNick != client)
		return (rfc::NICKINUSE); // ERR_NICKCOLLISION
	client->setNick(tmpNick);

	if (client->getRegistrationFlags() != REG_DONE)
	{
		client->setRegistrationFlags(REG_NICK);
		if (DEBUG)
			std::cout << "[FD " << client->getFD()
				<< "] Nick registration successfull.\n";
		srv.tryCompleteRegistration(*client);
	}
	return (rfc::OK);
}
