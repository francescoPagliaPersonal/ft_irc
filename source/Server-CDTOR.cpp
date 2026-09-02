/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server-CDTOR.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fpaglia <fpaglia@student.42vienna.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/10 00:45:35 by mweghofe          #+#    #+#             */
/*   Updated: 2026/09/02 17:21:58 by fpaglia          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_irc.hpp"
#include "Response.hpp"
#include "Server.hpp"

#include <sys/socket.h> // socket, connect, getsockname
#include <arpa/inet.h>	// inet_addr, inet_ntop
#include <netinet/in.h> // AF_INET, sockaddr_in, htons
#include <unistd.h>		// close

namespace {
	std::string retrieveServerAddress();
	void printNetworkUsageInfo(const std::string&, int, const std::string&);
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
	//TODO: replace server hard coded label with official name.
	Response::init("CoolServ");
	// Client class
	// CommandDispatch class
	_cmdReg.registerCmds();
	std::string ip = retrieveServerAddress();
	printNetworkUsageInfo(ip, port, pw);
}

Server::~Server()
{
	while (!_clients.empty())
		_removeClient(_clients.begin()->second);
	while (!_channels.empty())
		_deleteChannel(_channels.begin()->second);
}

// -------------------------------------------------------------------------- //
// HOW TO USE THE SERVER
// -------------------------------------------------------------------------- //

namespace
{

// Resolves own IPv4 address via dummy UDP connection workaround.
std::string retrieveServerAddress()
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
	return (ip);
}

// Print welcome message with instructions.
void printNetworkUsageInfo(const std::string& ip, int port, const std::string& pw)
{
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
