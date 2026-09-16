/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_quit.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fpaglia <fpaglia@student.42vienna.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/25 17:17:17 by mweghofe          #+#    #+#             */
/*   Updated: 2026/09/16 10:46:18 by fpaglia          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Client.hpp"
#include "Command.hpp"
#include "IServerCtrl.hpp"
#include "Message.hpp"
#include "irc.hpp"
#include "Response.hpp"

#include <set>

rfc cmd_quit(IServerCtrl& srv, const Message& msg)
{
	std::set<Client*> contacts;
	Client* client = msg.sender;
	
	srv.removeClientFromAllChannels(client, &contacts);
	// if a param is passed and is not the trailing then it's passed as trailing.
	// fix included to satisfy incorrect format.
	if (msg.argCount() == 1 && !(msg.flags & irc::MSG_HAS_TRAILING))
		srv.broadcast(contacts, Response::buildRegular(msg, "", msg.params[0]));
	else
		srv.broadcast(contacts, Response::buildRegular(msg, ""));
	client->setQuit(true);
	return (irc::HASQUIT);
}
