/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CommandRegistry-handleProtocolErrors.cpp           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mweghofe <mweghofe@student.42vienna.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/17 16:52:31 by mweghofe          #+#    #+#             */
/*   Updated: 2026/08/18 15:44:31 by mweghofe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Client.hpp"
#include "CommandRegistry.hpp"
#include "IServerCtrl.hpp"
#include "ft_irc.hpp"
#include <sstream>

// Send the proper protocol reply for NUMERIC to the message's sender;
// returns false if the client should be disconnected.
void CommandRegistry::handleNumericResponses(IServerCtrl& srv, int numeric,
	const Message& msg)
{
	Client& client = *msg.sender;
	std::stringstream reply;
	if (numeric != rfc::OK)
	{
		reply << ":CoolServ " << numeric << ' ' << client.getNick() << ' ';
		std::map<t_uint, rfcResponse>::iterator it;
		// FIXME eventually this shouldn't be needed anymore
		it = _rfcCodes.find(numeric);
		if (it == _rfcCodes.end())
		{
			std::cout << "[Warning] " << __FUNCTION__
				<< " received a currently unknown protocol error: " << numeric
				<< std::endl;
			return ;
		}
		reply << _rfcCodes[numeric](msg);
		srv.sendMessage(client, reply.str());
	}
}
