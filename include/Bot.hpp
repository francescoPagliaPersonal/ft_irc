/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Bot.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mweghofe <mweghofe@student.42vienna.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/02 07:22:22 by mweghofe          #+#    #+#             */
/*   Updated: 2026/09/08 12:35:24 by mweghofe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BOT_HPP
# define BOT_HPP

# include "Epoll.hpp"
# include "irc.hpp"

# include <csignal>		// sig_atomic_t
# include <string>

# define SPAM_MSG_42 "42 is the answer to the Ultimate Question of Life, the Universe, and Everything."
# define SPAM_MSG_CH "Have you tried turning it off and on again?"
# define SPAM_USER "BugDetector"
# define SPAM_CHANNEL "#support"
# define EPOLL_FL_DEFAULT EPOLLIN | EPOLLERR | EPOLLHUP // duplicate of Server.hpp
# define BUF_SIZE 4095 // duplicate of Client.hpp

class Bot
{
	public:
		// ----
		Bot(const unsigned int, const unsigned short, const std::string&);
		~Bot();
		// ----
		void run();
	private:
		// ----
		int _fd;			// socket with connection to server
		unsigned int _server;
		unsigned short _port;
		std::string _pw;	// server password
		Epoll _epoll;
		bool _hasConn;
		std::string _bufIN;
		// ----
		static volatile std::sig_atomic_t _keepRunning;
		static void signalHandler(int);
		static bool _captureSignals();
		// ----
		void _connect();
		int _awaitHandshake() const;
		int _handshakeResult();
		void _sendToServer(const std::string&);
		void _registerWith(const std::string&);
		void _joinChannel(const std::string&);
		void _spamUser(const std::string&);
		void _spamChannel(const std::string&);
		irc::epollret _discardInput();
		irc::epollret _receiveToBuffer();
		void _processInputBuffer();
		// ----
		Bot();
		Bot(const Bot&);
		Bot& operator=(const Bot&);
};

#endif
