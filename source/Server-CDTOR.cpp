/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server-CDTOR.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mweghofe <mweghofe@student.42vienna.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/10 00:45:35 by mweghofe          #+#    #+#             */
/*   Updated: 2026/08/18 14:28:45 by mweghofe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

// -------------------------------------------------------------------------- //
// CUSTOM CTOR & DTOR
// -------------------------------------------------------------------------- //

// Custom constructor to start the server on PORT with PW.
// Also creates the listening socket, activates epoll system and registers
// the listening socket with epoll. Also set's up the signals.
Server::Server(int port, std::string pw)
	: _pw(pw)
	, _listener(port)
	, _clients()
	, _epoll()
	, _cmdReg()
{
	_epoll.add(_listener.getFD(), EPOLLIN);
	_captureSignals();
	// Client class
	// CommandDispatch class
	_cmdReg.registerCmds();
	_cmdReg.registerCodes();
}

Server::~Server()
{
	while (!_clients.empty())
		_removeClient(_clients.begin()->second);
	while (!_channels.empty())
		_removeChannel(_channels.begin()->second);
}
