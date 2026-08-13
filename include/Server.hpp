/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mweghofe <mweghofe@student.42vienna.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/09 14:32:56 by mweghofe          #+#    #+#             */
/*   Updated: 2026/08/13 20:35:34 by mweghofe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
# define SERVER_HPP

// -------------------------------------------------------------------------- //

# include <csignal>
#include <deque>
# include <string>
# include <map>
#include "Message.hpp"
# include "ft_irc.hpp"
# include "ListeningSocket.hpp"
# include "Client.hpp"
# include "CommandRegistry.hpp"
# include "Epoll.hpp"
# include "Channel.hpp"

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
		std::string				_pw;		// connection password
		ListeningSocket			_listener;	// server's own listening socket
		std::map<int, Client*>	_clients;	// map of all Clients, sorted by FD
		std::deque<Message>		_msgsQueue;
		std::map<std::string, Channel*> _channels; // Channels sorted by title
		Epoll					_epoll;		// isolated kernel epoll wrapper
		CommandRegistry			_cmdReg;	// command orchestrator
		// ----
		static volatile std::sig_atomic_t _isAlive;	// server state
		static void signalHandler(int);
		// ----
		void _captureSignals();
		void _handleListenEvent();
		void _handleClientEvent(epoll_event&);
		void _registerNewClient(int, const sockaddr_in&);
		void _removeClient(Client*);
		void _executeCommands();
		bool _processInputBuffer(Client*);
		void _removeMsgsFromSuspicious(Client*);
		void _addChannel(const std::string&);
		void _removeChannel(const std::string&);
		void _removeChannel(Channel*);
		// ---- TESTING
		void _addTwoChannels(const std::string&, const std::string&);
		void _addClients2Channels();
		void _printAll();
		void _removeClientsFromChannel(const std::string&);
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
