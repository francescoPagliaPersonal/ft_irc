/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_privmsg.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mweghofe <mweghofe@student.42vienna.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/19 15:31:28 by mweghofe          #+#    #+#             */
/*   Updated: 2026/08/19 16:12:28 by mweghofe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Command.hpp"
#include "IServerCtrl.hpp"
#include "Client.hpp"

int cmd_privmsg(IServerCtrl& srv, const Message& msg)
{
	std::string reply;
	Client* sender = msg.sender;
	// TODO do we want proper return codes ie. 401, 411,412 ERR_NOTEXTTOSEND
	// TODO arguments for privmsg .. currently this doesn trigger
	if (!(msg.flags & MSG_HAS_PARAMS))
		return (rfc::NORECIPIENT);
	if (!(msg.flags & MSG_HAS_TRAILING))
		return (rfc::NOTEXT);
	Client* recipient = srv.findClientByNick(msg.params[0]);
	if (recipient == NULL)
		return (rfc::NOSUCHNICK);
	reply = ':' + sender->getNick() + '!' + sender->getUserName();
	// FIXME still needs the ADDR stuff and getAddr or getHost
	// reply += '@' + sender->getHost
	reply += " PRIVMSG " + msg.params[0] + " :" + msg.trailing + CRLF;
	srv.sendMessage(*recipient, reply);
	return (rfc::OK);
}
