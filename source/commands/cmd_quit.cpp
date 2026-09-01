/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_quit.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mweghofe <mweghofe@student.42vienna.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/25 17:17:17 by mweghofe          #+#    #+#             */
/*   Updated: 2026/09/01 09:28:48 by mweghofe         ###   ########.fr       */
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
		(*it)->pushMembersToSet(contacts);
		(*it)->removeClient(client);
		client->removeChannel((*it));
	}
	std::set<Client*>::iterator it_c;
	for (it_c = contacts->begin(); it_c != contacts->end(); it_c++)
	{
		if (*it_c == client)
		{
			contacts->erase(it_c);
			break ;
		}
	}
}

} // end of namespace

rfc cmd_quit(IServerCtrl& srv, const Message& msg)
{
	std::set<Client*> contacts;
	Client* client = msg.sender;
	// 1) default reply back to sender
	// => repurposed numeric, generic handler will do that instead
	// 2) collect clients of all subscribed channels and unregister with them
	processMemberships(client, &contacts);	
	// 3) broadcast QUIT to all relevant clients
	srv.broadcast(contacts, Response::buildRegular(msg, ""));
	// 4) internal stuff
	client->setQuit(true);
	return (irc::HASQUIT);
}
