/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mweghofe <mweghofe@student.42vienna.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/09 14:32:56 by mweghofe          #+#    #+#             */
/*   Updated: 2026/08/11 12:59:05 by mweghofe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
# define SERVER_HPP

// -------------------------------------------------------------------------- //

# include <csignal>
# include <string>
# include <map>
# include "ft_irc.hpp"
# include "ListeningSocket.hpp"
# include "Client.hpp"
# include "CommandRegistry.hpp"
# include "Epoll.hpp"

// -------------------------------------------------------------------------- //

# define DEF_EPOLL_FL EPOLLIN | EPOLLERR | EPOLLHUP

// -------------------------------------------------------------------------- //

class Server
{
	public:
		// ----
		Server(int, std::string);
		~Server();
		// ----
		// ----
		void run();
	private:
		// ----
		unsigned short			port_;		// own listening port
		int						nextClient_;// next available Client ID
		std::string				pw_;		// connection password
		ListeningSocket			listener_;	// server's own listening socket
		std::map<int, Client*>	clients_;	// map of all Clients, sorted by FD
		Epoll					epoll_;		// isolated kernel epoll wrapper
		CommandRegistry			cmdReg_;	// command orchestrator
		// ----
		static volatile std::sig_atomic_t isAlive_;	// server state
		static void signalHandler(int);
		// ----
		void captureSignals();
		void handleListenEvent();
		void handleClientEvent(epoll_event&);
		void registerNewClient(int, const sockaddr_in&);
		// ----
		Server();
		Server(const Server&);
		Server operator=(const Server&);
};

// -------------------------------------------------------------------------- //

#endif

/*
	FROM FIRST DESIGN PLANNING STAGE

-map<string, IPRecord*> ipRecords
-map<int, Client*> clients
-map<string, Channel*> channels
-vector<string> banned

+isBanned(...) : bool
+registerClient(...) : bool
+deleteClient(...) : bool
+addChannel(...) : bool
+removeChannel(...) : bool
+findClientByNick(...) : bool
+findClientByID(...) : bool
+sendToClient(...) : boo

*/
