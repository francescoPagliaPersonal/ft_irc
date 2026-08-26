/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_pass.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mweghofe <mweghofe@student.42vienna.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/12 11:42:55 by fpaglia           #+#    #+#             */
/*   Updated: 2026/08/27 13:07:39 by mweghofe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Command.hpp"
#include "IServerCtrl.hpp"
#include "Client.hpp"
#include "ft_irc.hpp"
#include "irc.hpp"

// Verify the server password and set the PASSWD registration flag.
rfc cmd_pass(IServerCtrl & srv, const Message & msg)
{
	Client *client = msg.sender;
	if (msg.params[0] != srv.getPassword())
		return (irc::PASSWDMISMATCH);
	if (!client->setRegistrationFlags(REG_PASSWD))
		return (irc::ALREADYREGISTERED);
	if (DEBUG == debug::DETAILED)
		std::cout << "[FD " << client->getFD() 
			<< "] Server password correct.\n";
	if (!client->getCap())
			srv.tryCompleteRegistration(*client);
	return (irc::OK);
}
