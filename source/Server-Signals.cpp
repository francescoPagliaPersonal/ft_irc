/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server-Signals.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mweghofe <mweghofe@student.42vienna.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/17 16:36:55 by mweghofe          #+#    #+#             */
/*   Updated: 2026/09/16 14:44:26 by mweghofe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"
#include <cstring>

// -------------------------------------------------------------------------- //
// SIGNALS
// -------------------------------------------------------------------------- //

volatile std::sig_atomic_t Server::_isAlive = true;

// Signal handler for SIGINT & SIGTERM.
void Server::signalHandler(int)
{
	_isAlive = false;
}

// Configures and registers signal and signal handlers.
void Server::_captureSignals()
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
