/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_ping.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fpaglia <fpaglia@student.42vienna.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/16 20:13:56 by mweghofe          #+#    #+#             */
/*   Updated: 2026/08/17 14:53:21 by fpaglia          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Command.hpp"
#include "IServerCtrl.hpp"
#include "Client.hpp"

int cmd_ping(IServerCtrl& srv, const Message& msg)
{
	Client *client = msg.sender;
	
	std::string token;
	if (!msg.params.empty())
		token = msg.params[0];
	else
		token = msg.trailing;

	srv.sendMessage(*client,
		std::string(":CoolServ PONG CoolServ :") + token + CRLF);
	return (rfc::OK);
}
