/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_quit.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fpaglia <fpaglia@student.42vienna.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/25 17:17:17 by mweghofe          #+#    #+#             */
/*   Updated: 2026/09/02 17:15:57 by fpaglia          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Client.hpp"
#include "Command.hpp"
#include "IServerCtrl.hpp"
#include "irc.hpp"
#include "Response.hpp"

#include <set>

rfc cmd_quit(IServerCtrl& srv, const Message& msg)
{
	std::set<Client*> contacts;
	Client* client = msg.sender;
	
	srv.removeClientFromAllChannels(client, &contacts);
	srv.broadcast(contacts, Response::buildRegular(msg, ""));
	client->setQuit(true);
	return (irc::HASQUIT);
}
