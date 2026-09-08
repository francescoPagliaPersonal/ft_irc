/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Bot-Connection.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mweghofe <mweghofe@student.42vienna.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/08 13:46:16 by mweghofe          #+#    #+#             */
/*   Updated: 2026/09/08 14:06:59 by mweghofe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Bot.hpp"
#include "ft_irc.hpp"

#include <cstring>
#include <cerrno>

#include <sys/socket.h>
#include <unistd.h>			// sleep

// Retry failed connections with a fresh socket until connection is made.
void Bot::_waitForServer()
{
	// 1) wait until kernel finishes handshake
	//    NOTE: epoll returns ready==1 on BOTH success and failure.
	//    EPOLLOUT on success, EPOLLERR|EPOLLHUP on failure both wake wait().
	//    ready>0 only means handshake finished, NOT that it succeeded.
	//    Must check SO_ERROR next to distinguish.
	// 2) after handshake ask kernel for result
	// 3) if there was no connection, wait a bit and try again
	while (_keepRunning && !_hasConn)
		{
			int ready = _awaitHandshake();
			if (ready == 0)
				continue;
			if (_handshakeResult() == 0)
				break;
			sleep(3);
			_connect();
		}
}

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
