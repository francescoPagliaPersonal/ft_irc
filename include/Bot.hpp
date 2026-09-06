/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Bot.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mweghofe <mweghofe@student.42vienna.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/02 07:22:22 by mweghofe          #+#    #+#             */
/*   Updated: 2026/09/06 20:55:50 by mweghofe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BOT_HPP
# define BOT_HPP

# include "Epoll.hpp"

# include <string>

# define SPAM_MSG_42 "42 is the answer to the Ultimate Question of Life, the Universe, and Everything."
# define SPAM_MSG_CH "Have you tried turning it off and on again?"
# define SPAM_USER "BugDetector"
# define SPAM_CHANNEL "#support"

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
		std::string _pw;	// server password
		Epoll _epoll;
		// ----
		void _registerWith(const std::string&);
		void _spamUser(const std::string& = SPAM_USER);
		void _spamChannel(const std::string& = SPAM_CHANNEL);
		// ----
		Bot();
		Bot(const Bot&);
		Bot& operator=(const Bot&);
};

#endif
