/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fpaglia <fpaglia@student.42vienna.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/09 16:22:35 by mweghofe          #+#    #+#             */
/*   Updated: 2026/08/20 10:16:08 by fpaglia          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENT_HPP
# define CLIENT_HPP

// -------------------------------------------------------------------------- //

#include "ft_irc.hpp"
#include "Channel.hpp"
#include <vector>
#include <deque>

// -------------------------------------------------------------------------- //

# define BUF_SIZE 4095

// -------------------------------------------------------------------------- //

struct sockaddr_in;
// class Channel;

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
		int getFD() const;
		// ---- Buffer ----
		e_pollret receiveToBuffer();
		e_pollret sendFromBuffer();
		std::vector<std::string> getRawStrings();
		void putReply2Buff(const std::string&);
		// ---- Get ----
		int	getRegistrationFlags() const;
		std::string	getNick() const;
		std::string	getUserName() const;
		std::string	getRealName() const;
		bool getCap() const;
		// ---- Set ----
		bool setRegistrationFlags(int flags);
		void setNick(const std::string & str);
		void setUserName(const std::string & str);
		void setRealName(const std::string & str);
		void setCap(bool);
		// ---- Channels ----
		bool isChannelMember(Channel*) const;
		void addChannel(Channel*);
		void removeChannel(Channel*);
	private:
		// ----
		int _fd;
		// ----
		std::string _bufIN;					// continuous storage for input data
		std::string _bufOUT;				// continuous storage for output msg
		unsigned char _registrationFlags; // FIXME needs type from newer ft_irc.hpp on Channels branch
		bool		_capRequested;			// track if client requested CAP
		std::string		_nick;				// client's nick name
		std::string		_userName;			// client's user name
		std::string		_realName;			// client's real name
		std::deque<Channel*> _channels;	// channels the client is registered to
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
