/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Bot-Connection.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fpaglia <fpaglia@student.42vienna.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/08 13:46:16 by mweghofe          #+#    #+#             */
/*   Updated: 2026/09/10 15:57:56 by fpaglia          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Bot.hpp"
#include "ft_irc.hpp"
#include "irc.hpp"

#include <cstring>
#include <cerrno>

#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <fcntl.h>			// fcntl
#include <unistd.h>			// sleep

// Helper to configure the FD of the listening socket.
void configureFD(int fd)
{	
	std::cout << "[CONNECTION] marking fd non blocking." << std::endl;
	errno = 0;
	// 3) configure socketfd as non-blocking
	int flags = ::fcntl(fd, F_GETFL, 0);
	if (flags < 0 || ::fcntl(fd, F_SETFL, flags | O_NONBLOCK) < 0)
		throw std::runtime_error(
			std::string("Error on fcntl(): ") + std::strerror(errno));
}

int Bot::connectWithRetry(int maxAttempts, int delaySeconds)
{
    for (int attempt = 0; attempt < maxAttempts; ++attempt)
    {
		std::cout << "[CONNECTION]" << " connecting to server. attempt: " << attempt << std::endl;
        int fd = ::socket(AF_INET, SOCK_STREAM, 0);
        if (fd < 0)
            throw std::runtime_error(std::string("socket: ") + std::strerror(errno));

        struct sockaddr_in ipAddr;
        ipAddr.sin_family      = AF_INET;
        ipAddr.sin_port        = htons(_port);
        ipAddr.sin_addr.s_addr = _server;

        if (::connect(fd, reinterpret_cast<sockaddr*>(&ipAddr), sizeof(ipAddr)) == 0)
		{
			configureFD(fd);
			_epoll.add(_fd, EPOLL_FL_DEFAULT);
            return fd;   // success
		}

        // failed — clean up and wait before retrying
        ::close(fd);

        ::sleep(delaySeconds);
        // optionally log savedErrno here
    }
    return -1;
}

// Retry failed connections with a fresh socket until connection is made.
irc::epollret Bot::_waitForServer()
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
			if (ready < 0)
				return irc::RET_CLOSE;
			if (ready == 0)
				continue;
			if (_handshakeResult() == 0)
				break ;
		}
	return irc::RET_OK;
}

int Bot::_awaitHandshake() const
{
	struct epoll_event	ev;

	std::memset(&ev, 0, sizeof(ev));
	int ready = _epoll.wait(&ev, 1, 3000);
	if (ready < 0)
		std::cout << "[Bot] Handshake timeout: epool handshake error." << std::endl;
		// throw std::runtime_error(
		// 	std::string("epoll wait(): ") + std::strerror(errno));
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
