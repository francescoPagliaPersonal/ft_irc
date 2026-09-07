/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Bot-Signals.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mweghofe <mweghofe@student.42vienna.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/07 14:45:57 by mweghofe          #+#    #+#             */
/*   Updated: 2026/09/07 14:57:44 by mweghofe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Bot.hpp"

#include <cstring>		// memset
#include <csignal>		// for sig_atomic_t and all the SIG*
#include "signal.h"		// sigaction etc.

// -------------------------------------------------------------------------- //
// SIGNALS
// -------------------------------------------------------------------------- //

volatile std::sig_atomic_t Bot::_keepRunning = true;

// Signal handler for SIGINT & SIGTERM.
void Bot::signalHandler(int)
{
	_keepRunning = false;
}

// Configures and registers signal and signal handlers.
bool Bot::_captureSignals()
{
	struct sigaction sa;
	std::memset(&sa, 0, sizeof(sa));
	sa.sa_handler = signalHandler;
	::sigemptyset(&sa.sa_mask);
	sa.sa_flags = 0;
	// interrupt and termination
	if (::sigaction(SIGINT, &sa, NULL) == -1)
		return (false);
	if (::sigaction(SIGTERM, &sa, NULL) == -1)
		return (false);
	return (true);
}
