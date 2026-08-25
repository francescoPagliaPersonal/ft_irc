/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server-Run.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mweghofe <mweghofe@student.42vienna.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/11 12:06:37 by mweghofe          #+#    #+#             */
/*   Updated: 2026/08/25 11:10:29 by mweghofe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

#include <cstring>		// memset
#include <arpa/inet.h>	// inet_ntop, INET_ADDRSTRLEN
#include <netdb.h>		// getaddrinfo, freeaddrinfo, struct addrinfo
#include <netinet/in.h> // AF_INET, sockaddr_in


namespace {
	void howToUse(int, const std::string&);
}

// -------------------------------------------------------------------------- //
// MAIN LOOP
// -------------------------------------------------------------------------- //

// Main Server Interface.
// Listening Socket and epoll() registration
// has already be done by the constructor.
void Server::run()
{
	struct epoll_event ev[MAX_EVENTS]; // TODO do we need to zero that one?
	howToUse(_listener.getPort(), _pw);
	while (_isAlive)
	{
		int readyFDs = _epoll.wait(ev, MAX_EVENTS, TIMEOUT);
		if (readyFDs != 0 && DEBUG && ev->events & EPOLLIN)
			std::cout << std::endl;
		// 1) epoll() stuff
		for (int i = 0; i < readyFDs; i++)
		{
			// A) servers's own listening port
			if (ev[i].data.fd == _listener.getFD())
			{
				_handleListenEvent();
				continue ;
			}
			// B) normal client (EPOLLIN/EPOLLOUT, filling/emptying our buffers)
			_handleClientEvent(ev[i]);
		}
		// 2) work the command queue (execute msg queue, fill client write buff)
		_executeCommands();
		// 3) housekeeping (signal, timeout, sth else?)
		// TODO devise tasks to do
	}
}

// -------------------------------------------------------------------------- //
// HOW TO USE THE SERVER
// -------------------------------------------------------------------------- //

namespace
{

// Resolves and prints own IPv4 address with instructions how to connect.
void howToUse(int port, const std::string& pw)
{
	char hostname[256];
	std::string ip = "127.0.0.1"; // fallback value
	// 1) ask machine for local hostname
	if (gethostname(hostname, sizeof(hostname)) == 0)
	{
		struct addrinfo filter;
		struct addrinfo* res = NULL;
		// clear and set filter structure
		std::memset (&filter, 0, sizeof(filter));
		filter.ai_family = AF_INET;
		filter.ai_socktype = SOCK_STREAM;
		// 2) resolve hostname to a IPv4 socket address
		if (getaddrinfo(hostname, NULL, &filter, &res) == 0 && res != NULL)
		{
			char buf[INET_ADDRSTRLEN];
			struct sockaddr_in *addr;
			addr = reinterpret_cast<struct sockaddr_in*>(res->ai_addr);
			// 3) convert address into regular IP text notation
			if (inet_ntop(AF_INET, &addr->sin_addr, buf, sizeof(buf)) != NULL)
				ip = buf;
			freeaddrinfo(res);
		}
	}
	// print welcome message with instructions
	std::cout << "[Info] Server address: " << ip << ':' << port << '\n';
	std::cout << "[Info] Connect with irssi from terminal\n"
			  << "       locally:  irssi -c localhost -p " << port
			  << " -w " << pw << '\n'
			  << "       remotely: irssi -c " << ip << " -p " << port
			  << " -w " << pw << '\n';
	std::cout << "[Info] Connect with irssi from within irssi\n"
			  << "       locally:  /connect localhost "
			  << port << ' ' << pw << '\n'
			  << "       remotely: /connect " << ip << ' '
			  << port << ' ' << pw << '\n';
}

} // end of namespace
