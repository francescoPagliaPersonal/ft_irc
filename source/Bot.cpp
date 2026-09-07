/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Bot.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mweghofe <mweghofe@student.42vienna.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/02 08:12:43 by mweghofe          #+#    #+#             */
/*   Updated: 2026/09/07 12:16:58 by mweghofe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Bot.hpp"
#include "ft_irc.hpp"

#include <cstring>
#include <cerrno>

#include <sys/socket.h>

int Bot::_awaitHandshake() const
{
	struct epoll_event	ev;

	std::memset(&ev, 0, sizeof(ev));
	int ready = _epoll.wait(&ev, 1, 3000);
	if (ready < 0)
		throw std::runtime_error(
			std::string("epoll wait(): ") + std::strerror(errno));
	if (ready == 0)
		std::cout << "[Bot] Handshake timeout: no epoll event, handshake not finished." << std::endl;
	return (ready);
}

int Bot::_handshakeResult() const
{
	int err = 0;
	socklen_t len = sizeof(err);

	if (::getsockopt(_fd, SOL_SOCKET, SO_ERROR, &err, &len) < 0)
		throw std::runtime_error(
			std::string("getsockopt(): ") + std::strerror(errno));

	if (err != 0)
		std::cout << "[Bot] Connection to server failed: " << std::strerror(err) << std::endl;
	else
		std::cout << "[Bot] Connection to server succeeded." << std::endl;
	return (err);
}

void Bot::_registerWith(const std::string& name)
{
	std::string msg;
	
	msg.append("NICK " + name + CRLF);
	msg.append("USER " + name + " 0 * :" + name + CRLF);
	msg.append("PASS " + _pw + CRLF);

	errno = 0;
	if (::send(_fd, msg.c_str(), msg.size(), 0) < 0)
		std::cerr << "Error sending registration: " << std::strerror(errno) << std::endl;
	else
		std::cout << "Sent:\n" << msg;
}

void Bot::_joinChannel(const std::string& channel)
{
	std::string msg;
	msg.append("JOIN " + channel + CRLF);
	::send(_fd, msg.c_str(), msg.size(), 0);
}

void Bot::_spamUser(const std::string& nick)
{
	std::string msg;
	msg.append("PRIVMSG " + nick + " :" + SPAM_MSG_42 + CRLF);
	::send(_fd, msg.c_str(), msg.size(), 0);
}

void Bot::_spamChannel(const std::string& channel)
{
	std::string msg;
	msg.append("PRIVMSG " + channel + " :" + SPAM_MSG_CH + CRLF);
	::send(_fd, msg.c_str(), msg.size(), 0);
}
