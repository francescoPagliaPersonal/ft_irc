/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mweghofe <mweghofe@student.42vienna.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/09 16:22:35 by mweghofe          #+#    #+#             */
/*   Updated: 2026/08/11 13:50:53 by mweghofe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENT_HPP
# define CLIENT_HPP

// -------------------------------------------------------------------------- //

#include "ft_irc.hpp"

// -------------------------------------------------------------------------- //

# define BUF_SIZE 4095

// -------------------------------------------------------------------------- //

struct sockaddr_in;

class Client
{
	public:
		// ----
		Client(int, const sockaddr_in&, int);
		~Client();
		// ----
		// ----
		int getFD() const;
		e_pollret receiveToBuffer();
		e_pollret sendFromBuffer();
	private:
		// ----
		int fd_;
		int id_;
		// ----
		std::string bufIN_;
		std::string bufOUT_;
		// ----
		Client();
		Client(const Client&);
		Client operator=(const Client&);
};

// -------------------------------------------------------------------------- //

#endif

/*
	FROM FIRST DESIGN PLANNING STAGE

-int socket
-bool isIRCop
-bool isAway
-string nickName
-vector<Channel*> channels
-IPRecord* address
-vector<IPlcy*> policies

+joinChannel(...) : bool
+leaveChannel(...) : bool
+listChannels(...) : bool
+countChannels(...) : bool
+getNick() : bool
+setNick(...) : bool

*/
