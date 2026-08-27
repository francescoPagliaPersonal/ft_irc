/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mweghofe <mweghofe@student.42vienna.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/09 14:32:56 by mweghofe          #+#    #+#             */
/*   Updated: 2026/08/27 13:13:49 by mweghofe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
# define SERVER_HPP

// -------------------------------------------------------------------------- //

# include <csignal>
# include <deque>
# include <string>
# include <map>

# include "IServerCtrl.hpp"
# include "ListeningSocket.hpp"
# include "Epoll.hpp"
# include "Client.hpp"
# include "Message.hpp"
# include "Channel.hpp"
# include "CommandRegistry.hpp"

// -------------------------------------------------------------------------- //

# define DEF_EPOLL_FL EPOLLIN | EPOLLERR | EPOLLHUP

// -------------------------------------------------------------------------- //

class Server : public IServerCtrl
{
	public:
		// ----
		Server(int, std::string);
		~Server();
		// ---- Entry Point ----
		void run();
		// ---- Interface ----
		std::string getPassword() const;
		//		Clients
		void tryCompleteRegistration(Client*);
		void sendMessage(Client*, const std::string&);
		Client* findClientByNick(const std::string &);
		Channel* getChannelByTitle(std::string);
		//		Channels
		rfc addToChannel(Client*, const std::string&, const std::string&);
		void removeFromChannel(Client*, const std::string&, const std::string&);
		void broadcast(const std::string&, Channel*, Client*);
		void broadcast(const std::string&, Channel*);
		// void broadcast(const std::string&, std::deque<Client*>, Client*);
		
	private:
		// ----
		std::string				_pw;		// connection password
		ListeningSocket			_listener;	// server's own listening socket
		std::map<int, Client*>	_clients;	// map of all Clients, sorted by FD
		std::deque<Message>		_msgsQueue; // holds all incoming messages/loop
		std::map<std::string, Channel*> _channels; // Channels sorted by title
		Epoll					_epoll;		// isolated kernel epoll wrapper
		CommandRegistry			_cmdReg;	// command orchestrator
		// ---- Signals ---
		static volatile std::sig_atomic_t _isAlive;	// server state
		static void signalHandler(int);
		void _captureSignals();
		// ---- Event Handler (epoll, buffers) ----
		void _handleListenEvent();
		void _handleClientEvent(epoll_event&);
		// ---- Clients ----
		void _registerNewClient(int, const sockaddr_in&);
		void _removeClient(Client*);
		void _disconnectClient(Client*);
		// ---- Command Execution ----
		void _executeCommands();
		bool _processInputBuffer(Client*);
		void _removeMsgsFromSuspicious(Client*);
		// ---- Channels ----
		void _removeChannel(const std::string&);
		void _removeChannel(Channel*);
		void _removeClientFromChannels(Client*);
		Channel* _getOrCreateChannel(const std::string&, const std::string&);
		Channel* _getChannel(const std::string&);
		Channel* _addChannel(const std::string& key, const std::string& title, const std::string& pw);
		Channel* _addChannel(const std::string& title, const std::string& pw);
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
