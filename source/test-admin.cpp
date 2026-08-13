/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test-admin.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mweghofe <mweghofe@student.42vienna.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/13 11:03:40 by mweghofe          #+#    #+#             */
/*   Updated: 2026/08/13 20:32:06 by mweghofe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"
#include <algorithm>

namespace test
{
void prtChannel(std::pair<const std::string, Channel*>& element)
{
	Channel* ch = element.second;
	std::cout << "[Channel] Title: " << ch->getTitle()
		<< " | Topic: " << ch->getTopic()
		<< " | Password: " << ch->getPassword();
}

} // end of namespace test


void Server::_addTwoChannels()
{
	_addChannel("One");
	_addChannel("Two");		
}

void Server::_addClients2Channels()
{
	Channel* ch;
	Client* cl;
	std::map<std::string, Channel*>::iterator it_ch;
	std::map<int, Client*>::iterator it_cl;
	for (it_ch = _channels.begin(); it_ch != _channels.end(); it_ch++)
	{
		ch = it_ch->second;
		for (it_cl = _clients.begin(); it_cl != _clients.end(); it_cl++)
		{
			cl = it_cl->second;
			ch->addClient(cl);
		}
	}
}

void Server::_removeClientsFromChannelOne()
{
	Channel* ch = _channels["One"];
	ch->removeMembers();
}

void Server::_printAll()
{
	Channel* ch;
	Client* cl;
	std::map<std::string, Channel*>::iterator it_ch;
	std::map<int, Client*>::iterator it_cl;
	for (it_ch = _channels.begin(); it_ch != _channels.end(); it_ch++)
	{
		ch = it_ch->second;
		std::cout << "[Channel] Title: " << ch->getTitle()
			<< " | Topic: " << ch->getTopic()
			<< " | Password: " << ch->getPassword() << std::endl;
		ch->prtMembers();
	}
	for (it_cl = _clients.begin(); it_cl != _clients.end(); it_cl++)
	{
		cl = it_cl->second;
		std::cout << "[Client] FD: " << cl->getFD() << std::endl;
	}
}

void Channel::prtMembers()
{
	const Client* cl;
	int i = 1;
	std::map<const Client*, bitMask>::iterator it_m;
	for (it_m = _members.begin(); it_m != _members.end(); it_m++)
	{
		cl = it_m->first;
		std::cout << "[#" << i << "] Client FD " << cl->getFD()
			<< " with privileges set (" << it_m->second << ")" << std::endl;
		i++;
	}
}

void Channel::removeMembers()
{
	while(!_members.empty())
		removeClient(_members.begin()->first);
}
