/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mweghofe <mweghofe@student.42vienna.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/09 16:22:35 by mweghofe          #+#    #+#             */
/*   Updated: 2026/08/16 13:29:27 by mweghofe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENT_HPP
# define CLIENT_HPP

// -------------------------------------------------------------------------- //

#include "ft_irc.hpp"
#include <vector>

// -------------------------------------------------------------------------- //

# define BUF_SIZE 4095

// -------------------------------------------------------------------------- //

struct sockaddr_in;

enum e_clientReg
{
	REG_PASSWD = 1 << 0,
	REG_USER = 1 << 1,
	REG_NICK = 1 << 2,
	REG_DONE = REG_PASSWD | REG_USER | REG_NICK
};

class Client
{
	public:
		// ----
		Client(int, const sockaddr_in&);
		~Client();
		// ----
		// ----
		int getFD() const;
		e_pollret receiveToBuffer();
		e_pollret sendFromBuffer();
		void debugWriteToBuffer(const std::string&);
		std::vector<std::string> getRawStrings();
		void putReply2Buff(const std::string&);
		// set
		bool setRegistrationFlags(int flags);
		std::string	getNick() const;
		std::string	getUserName() const;
		std::string	getRealName() const;
		// get
		int	getRegistrationFlags() const;
		void setNick(const std::string & str);
		void setUserName(const std::string & str);
		void setRealName(const std::string & str);
	private:
		// ----
		int _fd;
		// ----
		std::string _bufIN;
		std::string _bufOUT;
		unsigned char _registrationFlags; // FIXME needs type from newer ft_irc.hpp
		std::string		_nick;
		std::string		_userName;
		std::string		_realName;
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
