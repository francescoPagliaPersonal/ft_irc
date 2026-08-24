/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CommandRegistry-handleProtocolErrors.cpp           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mweghofe <mweghofe@student.42vienna.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/17 16:52:31 by mweghofe          #+#    #+#             */
/*   Updated: 2026/08/24 18:15:53 by mweghofe         ###   ########.fr       */
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
	if (!msg.sender)
		return (false);
	Client& client = *msg.sender;
	std::stringstream response;
	response << ":CoolServer" << " " << numeric << " " ;
	// TODO this is a proof of concept, requires a proper mechanism; some map perhaps with CODE + REPLY STRING
	switch (numeric)
	{
		case irc::OK: break ;
		// case 371:
		// 	srv.sendMessage(client, ":CoolServ 371 :A policy has not been respected.\r\n");
		// 	break ;
		case irc::BADCMD:
			response << msg.command;
			srv.sendMessage(client, response.str() + " :Command not found.\r\n");
			break ;
		case irc::NONICK:
			srv.sendMessage(client, response.str() + " :No nickname given.\r\n");
			break ;
		case irc::NICKINUSE:
			response << client.getNick() << " " << msg.params[0];
			srv.sendMessage(client, response.str() + " :Nickname is already in use.\r\n");
			break ;
		case irc::NICKBAD:
			response << client.getNick() << " " << msg.params[0];
			srv.sendMessage(client, response.str() + " :Erroneous Nickname.\r\n");
			break ;
		case irc::NOTREG:
			srv.sendMessage(client, ":CoolServ 451 <nick> :You have not registered.\r\n");
			break ;
		case irc::FEWPARAMS:
			response << client.getNick() << " " << msg.command;
			srv.sendMessage(client, response.str() + " :Not enough parameters.\r\n");
			break ;
		case irc::ALREADYREG:
			srv.sendMessage(client, ":CoolServ 462 <nick> :This user is already registered.\r\n");
			break ;
		case irc::BADPASS:
			srv.sendMessage(client, ":CoolServ 464 <nick> :Password incorrect.\r\n");
			break ;
		case irc::NOCONN:
			return (false); // FIXME this is a hard crash - we cannot remove a missing client w/o a pointer; catch elsewhere!
		default:
			std::cout << "[Warning] " << __FUNCTION__
				<< " received a currently unknown protocol error: " << numeric
				<< std::endl;
	}
	return (true);
}
