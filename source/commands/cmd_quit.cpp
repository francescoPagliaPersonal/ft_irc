/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_quit.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mweghofe <mweghofe@student.42vienna.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/25 17:17:17 by mweghofe          #+#    #+#             */
/*   Updated: 2026/08/28 08:41:06 by mweghofe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Command.hpp"
#include "IServerCtrl.hpp"
#include "Client.hpp"
#include "irc.hpp"
#include "Response.hpp"

#include <deque>
#include <set>

namespace
{

void processMemberships(Client* client, std::set<Client*>* contacts)
{
	std::deque<Channel*> joinedChannels;
	std::deque<Channel*>::const_iterator it;
	// std::map<Client*, bitMask> channelMembers;
	joinedChannels = client->getChannelsList();
	for (it = joinedChannels.begin(); it != joinedChannels.end(); it++)
	{
		// TODO remove these comments later on, when agreed upon
		//		effeciency wise, this could even do the cleanup...
		// this copies the whole map, potentially a lot of work
		// channelMembers = (*it)->getMembersMap();
		// let the channel do the work instead?
		(*it)->getMembers(contacts);
		(*it)->removeClient(client);
		client->removeChannel((*it));
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

} // end of namespace

rfc cmd_quit(IServerCtrl& srv, const Message& msg)
{
	std::set<Client*> contacts;
	Client* client = msg.sender;
	// 1) default reply back to sender
	// => repurposed numeric, generic handler will do that instead
	// 2) collect clients of all subscriped channels and unregister with them
	processMemberships(client, &contacts);	
	// 3) broadcast QUIT to all relevant clients
	notifyContacts(srv, contacts, msg);
	// 4) internal stuff
	client->setQuit(true);
	return (irc::HASQUIT);
}
