/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_ping.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mweghofe <mweghofe@student.42vienna.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/16 20:13:56 by mweghofe          #+#    #+#             */
/*   Updated: 2026/08/16 20:15:53 by mweghofe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Command.hpp"
#include "IServerCtrl.hpp"
#include "Client.hpp"

int cmd_ping(IServerCtrl& srv, const Message& msg)
{
	Client *client = msg.sender;
	if (client == NULL) // FIXME beware, this is broken; shouldn't even be possible?
		return (rfc::NOCONN);

	std::string token;
	if (!msg.params.empty())
		token = msg.params[0];
	else
		token = msg.trailing;

	srv.sendMessage(*client,
		std::string(":CoolServ PONG CoolServ :") + token + CRLF);
	return (rfc::OK);
}
