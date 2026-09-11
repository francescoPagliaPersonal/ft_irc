/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Bot-CDTOR.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fpaglia <fpaglia@student.42vienna.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/02 08:20:34 by mweghofe          #+#    #+#             */
/*   Updated: 2026/09/11 16:50:28 by fpaglia          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Bot.hpp"
#include "irc.hpp"

#include <stdexcept>

Bot::Bot(const irc::uint server,
		 const irc::uint16 port,
		 const std::string& pw)
	: _fd(-1)
	, _server(server)
	, _port(port)
	, _pw(pw)
	, _hasConn(false)
	, _joinSrv(false)
	, _joinDefChan(false)
{
	if (!_installSignals())
		throw std::runtime_error("Setting up signal handler failed.");
}


Bot::~Bot()
{
	::close(_fd);
}
