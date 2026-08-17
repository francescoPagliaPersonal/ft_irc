/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mweghofe <mweghofe@student.42vienna.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/09 14:42:26 by mweghofe          #+#    #+#             */
/*   Updated: 2026/08/17 13:48:15 by mweghofe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

#include <sstream>
// #include <arpa/inet.h> // for the commented out block below

// -------------------------------------------------------------------------- //
// OPERATION
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
		_epoll.add(fd, DEF_EPOLL_FL, tmp);
	}
	catch (const std::exception& e)
	{
		delete tmp;
		throw; // TODO currently this is a hard shutdown; wants sth else
	}
	_clients[fd] = tmp;
}

// Remove a client and deregister FD.
void Server::_removeClient(Client* client)
{
	_epoll.del(client->getFD());
	_clients.erase(client->getFD());
	delete client;
}

// Disconnects a client: remove all pending msgs, channels then client itself.
void Server::_disconnectClient(Client* client)
{
	// TODO finish this (channels, what else?)
	std::cout << "[Warning] Client removal requested for FD " << client->getFD()
		<< ". NOT FULLY IMPLEMENTED yet.\n";
	_removeMsgsFromSuspicious(client);
	_removeClient(client);
}

// Lookup client by NICK and return its pointer. Returns NULL if nothing found.
Client* Server::findClientByNick(const std::string & nick)
{
	std::map<int, Client *>::iterator it = _clients.begin();
	while (it != _clients.end())
	{

		if (it->second->getNick() == nick)
			return it->second;
		++it;
	}
	return NULL;
}

void Server::sendMessage(Client& client, const std::string& str)
{
	// TODO drop connection if bufOUT grows too much? or do we drop if kernel buffer stays full?
	client.putReply2Buff(str);
	// TODO consider CATCH & disconnect
	_epoll.mod(client.getFD(), DEF_EPOLL_FL | EPOLLOUT, &client);
}

void Server::tryCompleteRegistration(Client& client)
{
	if (client.getRegistrationFlags() != REG_DONE)
		return ;
	std::stringstream ss;
	ss  << ":CoolServ 001 " << client.getNick()
		<< " :Welcome to the IRC "
		<< client.getNick() << "!" << client.getUserName() << CRLF;
		// << "@" << inet_ntoa(client.addr().sin_addr) << "\r\n";
	std::cout << "[FD " << client.getFD() << "] User registration completed.\n";
	sendMessage(client, ss.str());
}

std::string Server::getPassword() const
{
	return (_pw);
}

// Add a new channel with TITLE.
void Server::_addChannel(const std::string& title)
{
	Channel* newCh = new Channel(title, "");
	_channels[title] = newCh;
	if (DEBUG)
		std::cout << "[Channel] '" << newCh->getTitle() << "' added.\n";
}

// Remove a channel by channel pointer.
void Server::_removeChannel(Channel* channel)
{
	_channels.erase(channel->getTitle());
	delete channel;
}

// Remove a channel by title.
void Server::_removeChannel(const std::string& title)
{
	Channel* channel = _channels[title];
	_channels.erase(title);
	delete channel;
}

// -------------------------------------------------------------------------- //
// OCF
// -------------------------------------------------------------------------- //

Server::Server()
	: _pw("")
	, _listener(-1)
	, _clients()
	, _epoll()
	, _cmdReg()
{}

Server::Server(const Server& other)
	: _pw("")
	, _listener(-1)
	, _clients()
	, _epoll()
	, _cmdReg()
{
	(void) other;
}

Server Server::operator=(const Server& other)
{
	(void) other;
	return (*this);
}

Server::~Server()
{
	while (!_clients.empty())
		_removeClient(_clients.begin()->second);
	while (!_channels.empty())
		_removeChannel(_channels.begin()->second);
}
