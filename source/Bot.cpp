/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Bot.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mweghofe <mweghofe@student.42vienna.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/02 08:12:43 by mweghofe          #+#    #+#             */
/*   Updated: 2026/09/07 16:47:03 by mweghofe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Bot.hpp"
#include "ft_irc.hpp"

#include <cstring>
#include <cerrno>
#include <csignal>

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

int Bot::_handshakeResult()
{
	int err = 0;
	socklen_t len = sizeof(err);

	if (::getsockopt(_fd, SOL_SOCKET, SO_ERROR, &err, &len) < 0)
		throw std::runtime_error(
			std::string("getsockopt(): ") + std::strerror(errno));

	if (err != 0)
		std::cout << "[Bot] Connection to server failed: " << std::strerror(err) << std::endl;
	else
	{
		std::cout << "[Bot] Connection to server succeeded." << std::endl;
		_hasConn = true;
		_epoll.mod(_fd, EPOLL_FL_DEFAULT, NULL);
	}
	return (err);
}

void Bot::_sendToServer(const std::string& msg)
{
	errno = 0;
	if (::send(_fd, msg.c_str(), msg.size(), MSG_NOSIGNAL) < 0)
	{
		std::cerr << "[Error] Sending to server: "
				  << errno << ", " << strerror(errno) << std::endl;
		// _keepRunning = false;
		_hasConn = false;
		if (errno == EPIPE)
			_connect();
	}
	// else
	// 	std::cout << "Sent:\n" << msg;
}

void Bot::_registerWith(const std::string& name)
{
	std::string msg;
	
	msg.append("NICK " + name + CRLF);
	msg.append("USER " + name + " 0 * :" + name + CRLF);
	msg.append("PASS " + _pw + CRLF);

	_sendToServer(msg);
}

void Bot::_joinChannel(const std::string& channel)
{
	_sendToServer("JOIN " + channel + CRLF);
}

void Bot::_spamUser(const std::string& nick)
{
	_sendToServer("PRIVMSG " + nick + " :" + SPAM_MSG_42 + CRLF);
}

void Bot::_spamChannel(const std::string& channel)
{
	_sendToServer("PRIVMSG " + channel + " :" + SPAM_MSG_CH + CRLF);
}

irc::epollret Bot::_discardInput()
{
	char buffer[4096];
	ssize_t bytes = ::recv(_fd, buffer, sizeof(buffer), 0);

	if (bytes <= 0)
		return (irc::RET_CLOSE);
	return (irc::RET_OK);
}

void Bot::_processInputBuffer()
{}
