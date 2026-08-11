/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server-Init.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mweghofe <mweghofe@student.42vienna.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/10 00:45:35 by mweghofe          #+#    #+#             */
/*   Updated: 2026/08/11 17:38:16 by mweghofe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"
#include <cstring>

// -------------------------------------------------------------------------- //
// SIGNALS
// -------------------------------------------------------------------------- //

volatile std::sig_atomic_t Server::_isAlive = true;

void Server::signalHandler(int)
{
	_isAlive = false;
}

void Server::captureSignals() // TODO this uses <signal.h> not <csignal>! is okay, but make sure what to use
{
	struct sigaction sa;
	std::memset(&sa, 0, sizeof(sa));
	sa.sa_handler = signalHandler;
	::sigemptyset(&sa.sa_mask);
	sa.sa_flags = 0;
	// interrupt and termination
	::sigaction(SIGINT, &sa, NULL);
	::sigaction(SIGTERM, &sa, NULL);
	// pipe error from clients get caught via regular handleClientEvent
	sa.sa_handler = SIG_IGN;
	::sigaction(SIGPIPE, &sa, NULL);
}

// -------------------------------------------------------------------------- //
// CUSTOM CTOR
// -------------------------------------------------------------------------- //

Server::Server(int port, std::string pw)
	: _pw(pw)
	, _listener(port)
	, _clients()
	, _epoll()
	, _cmdReg()
{
	_epoll.add(_listener.getFD(), EPOLLIN);
	captureSignals();
	// Client class
	// CommandDispatch class
}
