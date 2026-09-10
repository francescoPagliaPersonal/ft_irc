/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Bot.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fpaglia <fpaglia@student.42vienna.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/02 07:22:22 by mweghofe          #+#    #+#             */
/*   Updated: 2026/09/10 14:23:57 by fpaglia          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BOT_HPP
# define BOT_HPP

# include "IBot.hpp"
# include "Epoll.hpp"
# include "irc.hpp"
# include "Message.hpp"

# include <csignal>		// sig_atomic_t
# include <string>
# include <deque>

# define SPAM_MSG_42 "42 is the answer to the Ultimate Question of Life, the Universe, and Everything."
# define SPAM_MSG_CH "Have you tried turning it off and on again?"
# define SPAM_USER "BugDetector"
# define DEFAULT_CHANNEL "#support"
# define EPOLL_FL_DEFAULT EPOLLIN | EPOLLERR | EPOLLHUP // duplicate of Server.hpp
# define BUF_SIZE 4095 // duplicate of Client.hpp

class Bot : public IBot
{
	public:
		// ---- Construction
		Bot(const irc::uint, const irc::uint16, const std::string&);
		~Bot();
		// ---- Operation
		void run();
		void sendMessage(const std::string&);
	private:
		// ----
		int 			_fd;			// socket with connection to server
		irc::uint		_server;
		irc::uint16		_port;
		std::string 	_pw;	// server password
		Epoll 			_epoll;
		bool 			_hasConn;
		std::string 	_bufIN;
		std::string 	_bufOUT;
		std::deque<Message>		_msgsQueue; // holds all incoming messages/loop
		// ---- Signals
		static volatile std::sig_atomic_t _keepRunning;
		static void signalHandler(int);
		static bool _installSignals();
		// ---- Construction
		void _connect();
		// ---- Connection

		int		connectWithRetry(int, int);
		irc::epollret _waitForServer();
		int _awaitHandshake() const;
		int _handshakeResult();
		// ---- Operation
		void _epollHandler();
		// ---- Buffer
		irc::epollret _receiveToBuffer();
		irc::epollret _sendFromBuffer();
		void _processInputBuffer();
		// ---- Actions
		void _registerWith(const std::string&);
		// ---- Legacy Actions
		void _sendToServer(const std::string&);
		void _joinChannel(const std::string&);
		void _spamUser(const std::string&);
		void _spamChannel(const std::string&);
		// ----
		Bot();
		Bot(const Bot&);
		Bot& operator=(const Bot&);
};

#endif
