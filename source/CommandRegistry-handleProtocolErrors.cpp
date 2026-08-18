/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CommandRegistry-handleProtocolErrors.cpp           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mweghofe <mweghofe@student.42vienna.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/17 16:52:31 by mweghofe          #+#    #+#             */
/*   Updated: 2026/08/20 00:13:13 by mweghofe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Client.hpp"
#include "CommandRegistry.hpp"
#include "IServerCtrl.hpp"
#include "ft_irc.hpp"
#include <sstream>

// Send the proper protocol reply for NUMERIC to the message's sender;
// returns false if the client should be disconnected.
bool CommandRegistry::handleProtocolErrors(IServerCtrl& srv, int numeric,
	const Message& msg)
{
	// TODO can the sender even be NULL?
	if (!msg.sender)
		return (false);
	// FIXME this is a hard crash - we cannot remove a missing client w/o a pointer; catch elsewhere!
	if (numeric == rfc::NOCONN)
		return (false);
	Client& client = *msg.sender;
	std::string reply(":CoolServ ");
	std::map<t_uint, rfcResponse>::iterator it;
	if (numeric != rfc::OK)
	{
		// FIXME eventually this shouldn't be needed anymore
		it = _rfcCodes.find(numeric);
		if (it == _rfcCodes.end())
		{
			std::cout << "[Warning] " << __FUNCTION__
				<< " received a currently unknown protocol error: " << numeric
				<< std::endl;
			return (true);
		}
		reply.append(_rfcCodes[numeric](msg, client.getNick()));
		srv.sendMessage(client, reply);
	}
	return (true);
}
