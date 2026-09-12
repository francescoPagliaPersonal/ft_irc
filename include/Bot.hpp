/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Bot.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mweghofe <mweghofe@student.42vienna.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/02 07:22:22 by mweghofe          #+#    #+#             */
/*   Updated: 2026/09/12 15:25:50 by mweghofe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BOT_HPP
# define BOT_HPP

# include "Channel.hpp"
# include "IBot.hpp"
# include "Epoll.hpp"
# include "irc.hpp"
# include "Message.hpp"
# include "CommandRegistry.hpp"

# include <csignal>		// sig_atomic_t
# include <string>
# include <deque>

# define BOT_NAME "Bot"
# define SPAM_MSG_42 "42 is the answer to the Ultimate Question of Life, the Universe, and Everything."
# define SPAM_MSG_CH "Have you tried turning it off and on again?"
# define SPAM_COUNT 12
# define DEFAULT_CHANNEL "#ssot"
# define DEFAULT_TOPIC "Single Source Of Truth :: request !help from the bot"
# define CONN_DELAY 5		// default delay between retries
# define CONN_MAX_RETRY 4	// max retries before delay is increased
# define CONN_MAX_DELAY	300 // max delay between retries

# define EPOLL_FL_DEFAULT EPOLLIN | EPOLLERR | EPOLLHUP // duplicate of Server.hpp
# define BUF_SIZE 4095 // duplicate of Client.hpp

class Bot : public IBot
{
	public:
		// ---- Construction
		Bot(const irc::uint, const irc::uint16, const std::string&);
		~Bot();
		void run();
		void sendMessage(const std::string&);
		void toggleMirror();
	private:
		// ----
		int 			_fd;			// socket with connection to server
		irc::uint		_server;
		irc::uint16		_port;
		std::string 	_pw;	// server password
		Epoll 			_epoll;
		bool 			_hasConn;
		bool			_joinedServer;
		bool			_mirrorMsg;
		std::string 	_bufIN;
		std::string 	_bufOUT;
		std::deque<Message>		_msgsQueue; // holds all incoming messages/loop
		CommandRegistry			_cmdReg;	// command orchestrator
		// ---- Signals
		static volatile std::sig_atomic_t _keepRunning;
		static void signalHandler(int);
		static bool _installSignals();

		// ---- Connection
		int		_connectWithRetry(int);

		// ---- Operation
		void _runUntilDisconnect();
		void _executeMessages();
		// ---- Buffer
		irc::epollret	_receiveToBuffer();
		irc::epollret	_sendFromBuffer();
		void 			_processInputBuffer();
		// ---- Actions
		void _registerWith(const std::string&);
		void _joinChannel(const std::string&);
		void _lockAndSetTopic(const std::string&);
		void _mirrorMessage(const Message&);
		void _processInvite(const Message&);
		void _pong(const Message&);
		void _welcomeUser(const Message&);
		// ----
		Bot();
		Bot(const Bot&);
		Bot& operator=(const Bot&);
};

#endif
