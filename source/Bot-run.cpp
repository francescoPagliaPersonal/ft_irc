/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Bot-run.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mweghofe <mweghofe@student.42vienna.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/02 09:28:34 by mweghofe          #+#    #+#             */
/*   Updated: 2026/09/06 20:40:07 by mweghofe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Bot.hpp"
#include "ft_irc.hpp"

#include <sys/socket.h>
#include <sys/epoll.h>

#include <cerrno>
#include <cstring>
#include <iostream>
#include <stdexcept>

#include <unistd.h> // sleep()

void	Bot::run()
{
	struct epoll_event	ev;

	std::memset(&ev, 0, sizeof(ev));

	// 1) wait until kernel finishes handshake
	//    NOTE: epoll returns ready==1 on BOTH success and failure.
	//    EPOLLOUT on success, EPOLLERR|EPOLLHUP on failure both wake wait().
	//    ready>0 only means handshake finished, NOT that it succeeded.
	//    Must check SO_ERROR next to distinguish.
	int ready = _epoll.wait(&ev, 1, 3000);
	if (ready < 0)
		throw std::runtime_error(
			std::string("epoll wait(): ") + std::strerror(errno));
	if (ready == 0)
	{
		std::cout << "Timeout: no epoll event, handshake not finished." << std::endl;
		return ;
	}

	// 2) handshake done, ask kernel for final result
	int err = 0;
	socklen_t len = sizeof(err);

	if (::getsockopt(_fd, SOL_SOCKET, SO_ERROR, &err, &len) < 0)
		throw std::runtime_error(
			std::string("getsockopt(): ") + std::strerror(errno));

	// 3) print result
	if (err != 0)
		std::cout << "Connect failed: " << std::strerror(err) << std::endl;
	else
		std::cout << "Connect succeeded." << std::endl;

	// 4) Register with server: NICK and USER commands
	if (err == 0)
		_registerWith("bot");

	// 5) Stay alive with dummy loop
	while (true)
	{
		sleep(2);
	}
}
