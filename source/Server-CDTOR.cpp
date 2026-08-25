/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server-CDTOR.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mweghofe <mweghofe@student.42vienna.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/10 00:45:35 by mweghofe          #+#    #+#             */
/*   Updated: 2026/08/25 11:48:21 by mweghofe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

#include <sys/socket.h> // socket, connect, getsockname
#include <arpa/inet.h>	// inet_addr, inet_ntop
#include <netinet/in.h> // AF_INET, sockaddr_in, htons
#include <unistd.h>		// close

namespace {
	void howToUse(int, const std::string&);
}

// -------------------------------------------------------------------------- //
// CUSTOM CTOR & DTOR
// -------------------------------------------------------------------------- //

// Custom constructor to start the server on PORT with PW.
// Also creates the listening socket, activates epoll system and registers
// the listening socket with epoll. Also set's up the signals.
Server::Server(int port, std::string pw)
	: _pw(pw)
	, _listener(port)
	, _clients()
	, _epoll()
	, _cmdReg()
{
	_epoll.add(_listener.getFD(), EPOLLIN);
	_captureSignals();
	// Client class
	// CommandDispatch class
	_cmdReg.registerCmds();
	howToUse(port, pw);
}

Server::~Server()
{
	while (!_clients.empty())
		_removeClient(_clients.begin()->second);
	while (!_channels.empty())
		_removeChannel(_channels.begin()->second);
}

// -------------------------------------------------------------------------- //
// HOW TO USE THE SERVER
// -------------------------------------------------------------------------- //

namespace
{

// Resolves and prints own IPv4 address with instructions how to connect.
void howToUse(int port, const std::string& pw)
{
	std::string ip = "127.0.0.1"; // fallback value
	// 1) create a temporary UDP socket
	//    - workaround to connect() to a fake public address w/o sending packets
	//    - forces kernel to pick an outbound interface
	//    - thus getsockname() can retrieve a proper network address
	int fd = socket(AF_INET, SOCK_DGRAM, 0);
	if (fd >= 0)
	{
		// 2) build a dummy destination for the socket
		struct sockaddr_in remote = {};
		remote.sin_family = AF_INET;
		remote.sin_port = htons(42); // dummy target port
		remote.sin_addr.s_addr = inet_addr("42.42.42.42"); // dummy target ip
		// 3) connect the socket to the dummy destination
		if (connect(fd, reinterpret_cast<struct sockaddr*>(&remote),
				sizeof(remote)) == 0)
		{
			// 4) fetch local network address for this connection
			struct sockaddr_in local = {};
			socklen_t len = sizeof(local);
			if (getsockname(fd,
					reinterpret_cast<struct sockaddr*>(&local),
					&len) == 0)
			{
				// 5) transform into string and store it
				char buf[INET_ADDRSTRLEN];
				if (inet_ntop(AF_INET, &local.sin_addr, buf, sizeof(buf)) != NULL)
					ip = buf;
			}
		}
		close(fd);
	}

	// print welcome message with instructions
	std::cout << "[Info] Server address: " << ip << ':' << port << '\n';
	std::cout << "[Info] Connect with irssi from terminal\n" << COL_CYAN
			  << "       locally:  " << COL_RESET << "irssi -c localhost"
			  << " -p " << port << " -w " << pw << '\n' << COL_GREEN
			  << "       remotely: " << COL_RESET << "irssi -c " << ip
			  << " -p " << port << " -w " << pw << '\n';
	std::cout << "[Info] Connect with irssi from within irssi\n" << COL_CYAN
			  << "       locally:  " << COL_RESET << "/connect localhost "
			  << port << ' ' << pw << '\n' << COL_GREEN
			  << "       remotely: " << COL_RESET << "/connect " << ip << ' '
			  << port << ' ' << pw << COL_RESET << '\n';
	std::cout << "[Info] Connect with nc\n" << COL_CYAN
			  << "       locally:  " << COL_RESET << "nc -C localhost "
			  << port << '\n' << COL_GREEN
			  << "       remotely: " << COL_RESET << "nc -C " << ip << ' '
			  << port << COL_RESET << '\n';
}

} // end of namespace
