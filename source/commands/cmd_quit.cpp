/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_quit.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mweghofe <mweghofe@student.42vienna.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/25 17:17:17 by mweghofe          #+#    #+#             */
/*   Updated: 2026/08/25 17:23:11 by mweghofe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Command.hpp"
#include "IServerCtrl.hpp"
#include "Client.hpp"
#include "irc.hpp"

rfc cmd_quit(IServerCtrl& srv, const Message& msg)
{
	Client* client = msg.sender;
	std::string reply;
	// TODO needs new getFullUser()
	reply = ":" + client->getNick() + " QUIT :" + msg.trailing + CRLF;
	srv.sendMessage(*client, reply);
	return (irc::OK);
}
