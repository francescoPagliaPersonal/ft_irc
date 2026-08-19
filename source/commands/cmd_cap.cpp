/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_cap.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mweghofe <mweghofe@student.42vienna.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/12 13:54:12 by fpaglia           #+#    #+#             */
/*   Updated: 2026/08/19 12:27:13 by mweghofe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


# include "Command.hpp"
# include "IServerCtrl.hpp"
# include "Client.hpp"

/*
	CAPABILITY NEGOTIATION of IRCv3
	modern clients use it
	they querz optional features (which we don't need) BEFORE the registration
	and send a done request to finish registration
	which means => CAP END triggers tryCompleteRegistration
*/

// Handle the CAP command; currently a stub to advance the handshake.
int cmd_cap(IServerCtrl & srv, const Message & msg)
{
	// TODO need more content? currently is empty stub to advance handshake
	
	Client *client = msg.sender;
	if (msg.flags & MSG_HAS_PARAMS)
	{
		if (msg.params[0] == "LS")
		{
			if (client->getRegistrationFlags() != REG_DONE)
				client->setCap(true);
			std::string reply;
			reply = ":CoolServ CAP " + client->getNick() + " LS :" + CRLF;
			srv.sendMessage(*client, reply);
		}
		else if (msg.params[0] == "END" && client->getCap())
		{ // IF registration is NOT complete, cmd_* can still finish that step
				srv.tryCompleteRegistration(*client);
				client->setCap(false);
		}
		else
			return (rfc::INVALIDCAPCMD);
	}
	return (rfc::OK);
}
