/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_pass.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mweghofe <mweghofe@student.42vienna.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/12 11:42:55 by fpaglia           #+#    #+#             */
/*   Updated: 2026/08/16 14:28:28 by mweghofe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "Command.hpp"
# include "IServerCtrl.hpp"
# include "Client.hpp"

int cmd_pass(IServerCtrl & srv, const Message & msg)
{
	std::cout << "executing" << msg.command << std::endl;

	Client *client = msg.sender;
	if (client == NULL)
		return 1; // ERR_HANGUP??
	if (msg.params[0] == srv.getPassword())
	{
		client->setRegistrationFlags(REG_PASSWD);
	}
	return 0;
}