/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server-Clients.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mweghofe <mweghofe@student.42vienna.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/17 16:27:31 by mweghofe          #+#    #+#             */
/*   Updated: 2026/08/28 08:48:11 by mweghofe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_irc.hpp"
#include "Channel.hpp"
#include "Server.hpp"

#include <cstddef>
#include <deque>

// -------------------------------------------------------------------------- //
// INTERFACE -- CLIENTS
// -------------------------------------------------------------------------- //

// Lookup client by NICK and return its pointer. Returns NULL if nothing found.
Client* Server::findClientByNick(const std::string & nick) const
{
	std::map<int, Client *>::const_iterator it = _clients.begin();
	while (it != _clients.end())
	{

		if (it->second->getNick() == nick)
			return it->second;
		++it;
	}
	return NULL;
}

// Queue STR for sending to CLIENT and enable the EPOLLOUT interest.
void Server::sendMessage(Client* client, const std::string& str) const 
{
	if (!client->isBufferOutFilled() && !client->hasQuit())
		_epoll.mod(client->getFD(), EPOLL_FL_DEFAULT | EPOLLOUT, client);
	// TODO drop connection if bufOUT grows too much? or do we drop if kernel buffer stays full?
	client->putReply2Buff(str);
	// TODO consider CATCH & disconnect
}

// -------------------------------------------------------------------------- //
// PRIVATE -- CLIENTS
// -------------------------------------------------------------------------- //

// Creates new client and registers FD with epoll.
void Server::_registerNewClient(int fd, const struct sockaddr_in& addr)
{
	Client* tmp;
	// new client, add to map, do sth with addr? IPRecord class?
	try
	{
		tmp = new Client(fd, addr);
	}
	catch (const std::exception& e)
	{
		::close(fd);
		throw; // TODO currently this is a hard shutdown; wants sth else
	}
	// register epoll fd
	try
	{
		_epoll.add(fd, EPOLL_FL_DEFAULT, tmp);
	}
	catch (const std::exception& e)
	{
		delete tmp;
		throw; // TODO currently this is a hard shutdown; wants sth else
	}
	_clients[fd] = tmp;
	std::cout << "[Info] New connection from " << tmp->getHost()
			<< " accepted at FD " << fd << '\n';
}
void Server::_removeClientFromChannels(Client * client)
{
	std::deque<Channel *> channels = client->getChannelsList();
	for (size_t i = 0; i < channels.size(); ++i)
	{
		std::map<std::string, Channel*>::iterator it;
		it = _channels.find(Channel::title2key(channels[i]->getTitle()));
		if (it == _channels.end())
			continue;
		it->second->removeClient(client);
		client->removeChannel(it->second);
	}
}

// Remove a client and deregister FD.
void Server::_removeClient(Client* client)
{
	_epoll.del(client->getFD());
	_removeClientFromChannels(client);
	// TODO remove from _connections
	_clients.erase(client->getFD());
	delete client;
}

// Disconnects a client: remove all pending msgs, channels then client itself.
void Server::_prepareClientDisconnect(Client* client)
{
	// TODO finish this (channels, what else?)
	std::cout << "[Warning] Client removal requested for FD " << client->getFD()
		<< ". NOT FULLY IMPLEMENTED yet.\n";
	_removeMsgsFrom(client);
	_epoll.mod(client->getFD(), EPOLL_FL_QUIT, client);
	// HACK only for testing!!
	// _removeClient(client);
}
