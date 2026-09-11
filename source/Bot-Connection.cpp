/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Bot-Connection.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fpaglia <fpaglia@student.42vienna.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/08 13:46:16 by mweghofe          #+#    #+#             */
/*   Updated: 2026/09/11 17:01:12 by fpaglia          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Bot.hpp"
#include "ft_irc.hpp"

#include <cstring>
#include <cerrno>
#include <sys/socket.h>
#include <netdb.h>
#include <fcntl.h>			// fcntl

namespace  {
	
	// Helper to configure the FD of the listening socket.
	void configureFD(int fd)
	{	
		if (DEBUG)
			std::cout << "[CONNECTION] marking fd non blocking." << std::endl;
		errno = 0;
		// 3) configure socketfd as non-blocking
		int flags = ::fcntl(fd, F_GETFL, 0);
		if (flags < 0 || ::fcntl(fd, F_SETFL, flags | O_NONBLOCK) < 0)
			throw std::runtime_error(
				std::string("Error on fcntl(): ")
					+ std::strerror(errno));
	}
}

int Bot::connectWithRetry(int maxAttempts, int delaySeconds)
{
    for (int attempt = 0; attempt < maxAttempts; ++attempt)
    {
		std::cout 
				<< "[CONNECTION] connecting to server. attempt n: " 
				<< attempt << std::endl;
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
			_epoll.add(fd, EPOLL_FL_DEFAULT);
			_hasConn = true;
            return fd;
		}

        ::close(fd);
        ::sleep(delaySeconds);
    }
    return -1;
}

