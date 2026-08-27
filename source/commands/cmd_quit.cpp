/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_quit.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mweghofe <mweghofe@student.42vienna.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/25 17:17:17 by mweghofe          #+#    #+#             */
/*   Updated: 2026/08/27 21:48:25 by mweghofe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Command.hpp"
#include "IServerCtrl.hpp"
#include "Client.hpp"
#include "irc.hpp"
#include "Response.hpp"

#include <deque>
#include <set>

void collectMembers(Client* client, std::set<Client*>* contacts)
{
	std::deque<Channel*> joinedChannels;
	std::deque<Channel*>::const_iterator it;
	// std::map<Client*, bitMask> channelMembers;
	joinedChannels = client->getChannelsList();
	for (it = joinedChannels.begin(); it != joinedChannels.end(); it++)
	{
		// TODO remove these comments later on, when agreed upon
		// this copies the whole map, potentially a lot of work
		// channelMembers = (*it)->getMembersMap();
		// let the channel do the work instead?
		(*it)->getMembers(contacts);
	}
}

void notifyContacts(IServerCtrl& srv, std::set<Client*>& contacts, const Message& msg)
{
	std::set<Client*>::const_iterator it;
	for (it = contacts.begin(); it != contacts.end(); it++)
	{
		if (*it != msg.sender)
			srv.sendMessage(*it, Response::buildRegular(msg, ""));
	}
}

rfc cmd_quit(IServerCtrl& srv, const Message& msg)
{
	std::string reply;
	std::set<Client*> contacts;
	Client* client = msg.sender;
	// 1) default reply back to sender
	reply = Response::buildError(msg, "Closing link", "Quit");
	srv.sendMessage(client, reply);
	// 2) look for channels & collect clients
	collectMembers(client, &contacts);	
	// 3) broadcast QUIT to all clients
	notifyContacts(srv, contacts, msg);
	// 4) internal stuff
	// TODO somehow handle disconnect
	return (irc::OK);
}

/*
> 2026/08/27 20:02:14.392414  length=15 from=183 to=197
QUIT :leaving\r
< 2026/08/27 20:02:14.392635  length=53 from=3328 to=3380
ERROR :Closing link: (mw@< 2026/08/27 20:02:14.392718  length=47 from=3179 to=3225
1:2B7u.g0D.e0t.e1c)1 o[rQ!umiwt@:1 2l7e.a0v.i0n.g1] \rQ
UIT :Quit: leaving\r

> 2026/08/27 20:07:13.467037  length=15 from=255 to=269
QUIT :leaving\r
< 2026/08/27 20:07:13.467366  length=53 from=3538 to=3590
ERROR :Closing lin< 2026/08/27 20:07:13.467479  length=47 from=3675 to=3721
k::B u(gmDwe@t1e2c71.o0r.!0m.w1@)1 2[7Q.u0i.t0:. 1l eQaUvIiTn g:]Q\ru
it: leaving\r

*/
