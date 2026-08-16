/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_nick.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mweghofe <mweghofe@student.42vienna.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/12 13:54:12 by fpaglia           #+#    #+#             */
/*   Updated: 2026/08/16 11:27:50 by mweghofe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


# include "Command.hpp"
# include "Server.hpp"
# include "Client.hpp"

int cmd_nick(IServerCtrl & srv, const Message & msg)
{
	// TODO: this is just a quick proof of concept more complex nick evaluation 
	// should be carried out (against the whole server).

	std::cout << "executing: " << msg.command << std::endl;

	Client *client = msg.sender;
	if (client == NULL)
		return 1; // ERR_HANGUP??
	std::string tmpNick = msg.params[0];
	Client *hasThisNick = srv.findClientByNick(tmpNick);
	if ( hasThisNick != NULL)
		return 1; // ERR_NICKCOLLISION
	client->setNick(tmpNick);
	
	if (client->getRegistrationFlags() != REG_DONE)
	{
		client->setRegistrationFlags(REG_NICK);
		if (client->getRegistrationFlags() & REG_NICK)
			std::cout 
				<< "Nick registration successfull." << std::endl;
	}
	return (0);
}
