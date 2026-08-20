/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_user.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mweghofe <mweghofe@student.42vienna.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/12 13:54:12 by fpaglia           #+#    #+#             */
/*   Updated: 2026/08/20 09:20:24 by mweghofe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "Command.hpp"
#include "IServerCtrl.hpp"
#include "Client.hpp"
#include "RFCnumeric.hpp"

// Set the client's user name and real name, and mark USER registration.
int cmd_user(IServerCtrl & srv, const Message & msg)
{
	// TODO: this is just a quick proof of concept more complex UserName evaluation 
	// should be carried out (against the whole server).
	// TODO also need charset validation?
	
	Client *client = msg.sender;
	
	if (!client->setRegistrationFlags(REG_USER))
		return (rfc::ALREADYREGISTERED);
	client->setUserName(msg.params[0]);
	client->setRealName(msg.trailing);
	if (DEBUG == debug::DETAILED)
		std::cout << "[FD " << client->getFD() 
			<< "] UserName registration successfull.\n";
	srv.tryCompleteRegistration(*client);

	return (rfc::OK);
}
