/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_pass.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fpaglia <fpaglia@student.42vienna.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/12 11:42:55 by fpaglia           #+#    #+#             */
/*   Updated: 2026/08/17 14:53:14 by fpaglia          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "Command.hpp"
# include "IServerCtrl.hpp"
# include "Client.hpp"
# include "ft_irc.hpp"

int cmd_pass(IServerCtrl & srv, const Message & msg)
{
	Client *client = msg.sender;
	if (msg.params[0] != srv.getPassword())
		return (rfc::BADPASS);
	if (!client->setRegistrationFlags(REG_PASSWD))
		return (rfc::ALREADYREG);
	if (DEBUG == debug::DETAILED)
		std::cout << "[FD " << client->getFD() 
			<< "] Server password correct.\n";
	srv.tryCompleteRegistration(*client);
	return (rfc::OK);
}
