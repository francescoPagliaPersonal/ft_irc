/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_ping.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mweghofe <mweghofe@student.42vienna.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/16 20:13:56 by mweghofe          #+#    #+#             */
/*   Updated: 2026/08/24 18:15:53 by mweghofe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Command.hpp"
#include "IServerCtrl.hpp"
#include "Client.hpp"
#include "irc.hpp"

// Answer a PING with a PONG carrying the same token back to the sender.
rfc cmd_ping(IServerCtrl& srv, const Message& msg)
{
	Client *client = msg.sender;
	
	std::string token;
	if (!msg.params.empty())
		token = msg.params[0];
	else
		token = msg.trailing;

	srv.sendMessage(*client,
		std::string(":CoolServ PONG CoolServ :") + token + CRLF);
	return (irc::OK);
}
