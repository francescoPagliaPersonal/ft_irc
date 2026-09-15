/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_whois.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mweghofe <mweghofe@student.42vienna.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/15 18:10:00 by mweghofe          #+#    #+#             */
/*   Updated: 2026/09/15 18:10:00 by mweghofe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Client.hpp"
#include "Command.hpp"
#include "IServerCtrl.hpp"
#include "Message.hpp"
#include "Response.hpp"
#include "irc.hpp"

#include <vector>

// Minimal WHOIS: answer irssi's probe on nick collision (433) with
// 311/312 per known nick (401 otherwise) plus a final 318.
// Deliberately open to unregistered clients, since the probe is
// sent during registration before REG_DONE.
rfc cmd_whois(IServerCtrl& srv, const Message& msg)
{
	Client* client = msg.sender;

	if (!(msg.flags & irc::MSG_HAS_PARAMS))
		return (irc::NEEDMOREPARAMS);
	// WHOIS [<server>] <nickmask>[,<nickmask>]: nicks are the last param.
	std::string nickList = msg.params[msg.params.size() - 1];
	std::vector<std::string> nicks = irc::strSplit(nickList, ',', false);
	if (nicks.empty())
		return (irc::NEEDMOREPARAMS);
	for (size_t i = 0; i < nicks.size(); ++i)
	{
		Client* target = srv.findClientByNick(nicks[i]);
		if (target == NULL)
		{
			srv.sendMessage(client,
				Response::buildNumeric(msg, irc::NOSUCHNICK, nicks[i]));
			continue ;
		}
		srv.sendMessage(client,
			Response::buildNumeric(msg, irc::WHOISUSER,
				target->getNick() + " " + target->getUserName()
				+ " " + target->getHost() + " *",
				target->getRealName()));
		srv.sendMessage(client,
			Response::buildNumeric(msg, irc::WHOISSERVER,
				target->getNick() + " " + Response::getServerName(),
				"ft_irc server"));
	}
	srv.sendMessage(client,
		Response::buildNumeric(msg, irc::ENDOFWHOIS, nicks[0]));
	return (irc::OK);
}
