/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mweghofe <mweghofe@student.42vienna.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/09 14:32:56 by mweghofe          #+#    #+#             */
/*   Updated: 2026/09/04 20:01:49 by mweghofe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
# define SERVER_HPP

// -------------------------------------------------------------------------- //

# include <csignal>
# include <ctime>
# include <deque>
# include <string>
# include <map>
# include <set>

# include "IServerCtrl.hpp"
# include "ListeningSocket.hpp"
# include "Epoll.hpp"
# include "Client.hpp"
# include "Message.hpp"
# include "Channel.hpp"
# include "CommandRegistry.hpp"
#include "irc.hpp"

// -------------------------------------------------------------------------- //

# define EPOLL_FL_DEFAULT EPOLLIN | EPOLLERR | EPOLLHUP
# define EPOLL_FL_QUIT EPOLLOUT | EPOLLERR | EPOLLHUP

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
		std::time_t getStartTime() const;
		//		Clients
		void tryCompleteRegistration(Client*) const;
		void sendMessage(Client*, const std::string&) const;
		Client* findClientByNick(const std::string &) const;
		Channel* getChannelByTitle(std::string) const;
		//		Channels
		rfc addToChannel(Client*, const std::string&, const std::string&);
		void removeClientFromChannel(Client*, Channel&, std::set<Client*>* );
		void removeClientFromAllChannels(Client *, std::set<Client*>* );
		void broadcast(Channel*, Client *, const std::string&) const;
		void broadcast(Channel*, const std::string&) const;
		void broadcast(std::set<Client*>&, const std::string&) const;
	private:
		// ----
		std::string				_pw;		// connection password
		ListeningSocket			_listener;	// server's own listening socket
		std::map<int, Client*>	_clients;	// map of all Clients, sorted by FD
		std::deque<Message>		_msgsQueue; // holds all incoming messages/loop
		std::map<std::string, Channel*> _channels; // Channels sorted by key
		Epoll					_epoll;		// isolated kernel epoll wrapper
		CommandRegistry			_cmdReg;	// command orchestrator
		std::time_t				_startTime; // start time of the server
		std::map<std::string, std::deque<Client*> >
								_IPrecords; // map of clients that share same IP 
		irc::uint				_maxClients; // max number of clients that can register
		mutable std::set<Client*> _toRemove; // lists clients to remove forcefully
		
		// ---- Signals ---
		static volatile std::sig_atomic_t _isAlive;	// server state
		static void signalHandler(int);
		void _captureSignals();
		// ---- Event Handler (epoll, buffers) ----
		void _handleListenEvent();
		void _handleClientEvent(epoll_event&);
		// ---- Clients ----
		bool _appendToIPrecords(Client*);
		void _removeFromIPrecords(Client*);
		void _registerNewClient(int, const sockaddr_in&);
		void _deleteClient(Client*);
		void _prepareClientDisconnect(Client*);
		void _housekeeping();
		void _addToRemove(Client*) const;
		// ---- Command Execution ----
		void _executeCommands();
		bool _processInputBuffer(Client*);
		void _removeMsgsFrom(Client*);
		// ---- Channels ----

		void	_deleteChannel(Channel*);
		void _removeClientFromChannels(Client*);
		Channel* _getOrCreateChannel(const std::string&, const std::string&);
		Channel* _getChannel(const std::string&) const;
		Channel* _addChannel(const std::string& key, const std::string& title, const std::string& pw);
		Channel* _addChannel(const std::string& title, const std::string& pw);
		// ---- other
		std::string _getStartTimeString() const;
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
