/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_cap.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mweghofe <mweghofe@student.42vienna.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/12 13:54:12 by fpaglia           #+#    #+#             */
/*   Updated: 2026/08/19 10:02:02 by mweghofe         ###   ########.fr       */
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
			srv.sendMessage(*client, ":CoolServ CAP * LS :\r\n");
		else if (msg.params[0] == "END")
			srv.tryCompleteRegistration(*client);
		// TODO some error on unknown command
	}
	return (rfc::OK);
}
