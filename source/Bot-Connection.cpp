/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Bot-Connection.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mweghofe <mweghofe@student.42vienna.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/08 13:46:16 by mweghofe          #+#    #+#             */
/*   Updated: 2026/09/08 13:51:48 by mweghofe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Bot.hpp"
#include "ft_irc.hpp"

#include <cstring>
#include <cerrno>

#include <sys/socket.h>

void Bot::_waitForServer()
{}

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
