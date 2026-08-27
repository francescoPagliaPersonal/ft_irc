/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_quit.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mweghofe <mweghofe@student.42vienna.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/25 17:17:17 by mweghofe          #+#    #+#             */
/*   Updated: 2026/08/27 19:55:16 by mweghofe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Command.hpp"
#include "IServerCtrl.hpp"
#include "Client.hpp"
#include "irc.hpp"
#include "Response.hpp"

rfc cmd_quit(IServerCtrl& srv, const Message& msg)
{
	Client* client = msg.sender;
	std::string reply;
	// TODO first version needs new getFullUser()
	// TODO figure out WHICH reply is fine... any seems to do
	// this is the reply shown in the example rfc2812/#section-3.1.7
	// although it comes w/o ERROR
	reply = Response::buildError(msg, "Closing link", "Quit");
	// this is the reply as done by inspircd -- NEEDS clientIP merged
	// reply = "ERROR :Closing link: (" + client->getNick()
	// 	+ "@" + client->getHost() + ") [QUIT: "
	// 	+ msg.trailing + "]" + CRLF;
	srv.sendMessage(client, reply);
	return (irc::OK);
}
