/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server-Init.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mweghofe <mweghofe@student.42vienna.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/10 00:45:35 by mweghofe          #+#    #+#             */
/*   Updated: 2026/08/11 14:42:06 by mweghofe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"
#include <cstring>

// -------------------------------------------------------------------------- //
// SIGNALS
// -------------------------------------------------------------------------- //

volatile std::sig_atomic_t Server::isAlive_ = true;

void Server::signalHandler(int)
{
	isAlive_ = false;
}

void Server::captureSignals()
{
	struct sigaction sa;
	std::memset(&sa, 0, sizeof(sa));
	sa.sa_handler = signalHandler;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = 0;
	sigaction(SIGINT, &sa, NULL);
	sigaction(SIGTERM, &sa, NULL);
}

// -------------------------------------------------------------------------- //
// CUSTOM CTOR
// -------------------------------------------------------------------------- //

Server::Server(int port, std::string pw)
	: port_(port)
	, nextClient_(1)
	, pw_(pw)
	, listener_(port)
	, clients_()
	, epoll_()
	, cmdReg_()
{
	epoll_.add(listener_.getFD(), EPOLLIN, NULL);
	captureSignals();
	// Client class
	// CommandDispatch class
}
