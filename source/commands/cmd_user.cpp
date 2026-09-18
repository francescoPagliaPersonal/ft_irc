/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_user.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fpaglia <fpaglia@student.42vienna.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/12 13:54:12 by fpaglia           #+#    #+#             */
/*   Updated: 2026/09/18 13:22:51 by fpaglia          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_irc.hpp"
#include "Command.hpp"
#include "IServerCtrl.hpp"
#include "Client.hpp"
#include "irc.hpp"
#include "Response.hpp"

// Set the client's user name and real name, and mark USER registration.
rfc cmd_user(IServerCtrl & srv, const Message & msg)
{
	
	Client *client = msg.sender;
	(void) srv;
	
	if (client->getRegistrationFlags() & REG_USER)
		return (irc::ALREADYREGISTERED);
	// do not accept the user if user or real name are not compliant with the generic rules
	if (!irc::isNameCompliant(msg.params[0], FORBIDDEN_NAME_CHAR))
	{
		srv.sendMessage(msg.sender, Response::buildNumeric(msg, irc::UNKNOWNERROR,
			"", "User Name not compliant.")
		);
		return (irc::OK);
	}
	if (!irc::isNameCompliant(msg.params[3], FORBIDDEN_REALNAME_CHAR))
		{
		srv.sendMessage(msg.sender, Response::buildNumeric(msg, irc::UNKNOWNERROR,
			"", "Real Name not compliant.")
		);
		return (irc::OK);
	}
	client->setUserName(msg.params[0].substr(0, MAX_NICKLEN));
	client->setRealName(msg.params[3].substr(0, MAX_NICKLEN));
	client->setRegistrationFlags(REG_USER);
	if (DEBUG == debug::DETAILED)
		std::cout << "[FD " << client->getFD() 
			<< "] UserName registration successfull.\n";
	if (!client->getCap())
			srv.tryCompleteRegistration(client);
	return (irc::OK);
}
