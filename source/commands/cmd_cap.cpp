/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_cap.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mweghofe <mweghofe@student.42vienna.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/12 13:54:12 by fpaglia           #+#    #+#             */
/*   Updated: 2026/08/19 09:54:43 by mweghofe         ###   ########.fr       */
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
	srv.sendMessage(*client, ":CoolServ CAP * LS :\r\n");
	
	return (rfc::OK);
}
