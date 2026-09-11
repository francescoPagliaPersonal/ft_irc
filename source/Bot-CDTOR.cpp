/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Bot-CDTOR.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mweghofe <mweghofe@student.42vienna.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/02 08:20:34 by mweghofe          #+#    #+#             */
/*   Updated: 2026/09/11 22:38:32 by mweghofe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Bot.hpp"
#include "irc.hpp"
#include "CommandRegistry.hpp"

#include <stdexcept>

Bot::Bot(const irc::uint server,
		 const irc::uint16 port,
		 const std::string& pw)
	: _fd(-1)
	, _server(server)
	, _port(port)
	, _pw(pw)
	, _hasConn(false)
	, _joinedServer(false)
	, _joinDefChan(false)
{
	if (!_installSignals())
		throw std::runtime_error("Setting up signal handler failed.");
	_cmdReg.registerBotCmds();
}


Bot::~Bot()
{
	::close(_fd);
}
