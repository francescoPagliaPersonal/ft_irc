/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_cap.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mweghofe <mweghofe@student.42vienna.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/12 13:54:12 by fpaglia           #+#    #+#             */
/*   Updated: 2026/08/20 09:19:25 by mweghofe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "Command.hpp"
#include "IServerCtrl.hpp"
#include "Client.hpp"
#include "RFCnumeric.hpp"

// Handle the CAP command; currently a stub to advance the handshake.
int cmd_cap(IServerCtrl & srv, const Message & msg)
{
	// TODO need more content? currently is empty stub to advance handshake
	
	Client *client = msg.sender;
	srv.sendMessage(*client, ":CoolServ CAP * LS :\r\n");
	
	return (rfc::OK);
}
