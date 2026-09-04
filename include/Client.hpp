/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mweghofe <mweghofe@student.42vienna.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/09 16:22:35 by mweghofe          #+#    #+#             */
/*   Updated: 2026/09/04 19:34:29 by mweghofe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENT_HPP
# define CLIENT_HPP

// -------------------------------------------------------------------------- //

# include "Channel.hpp" // does still include ft_irc

# include <vector>
# include <deque>
# include <ctime>

// -------------------------------------------------------------------------- //

# define BUF_SIZE 4095			// size for the receive buffer
# define MAX_BUF_SIZE 512000	// max size for IN/OUT buffer in Byte

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

enum e_buffer
{
	BUF_IN,
	BUF_OUT
};

class Client
{
	public:
		// ----
		Client(int, const sockaddr_in&);
		~Client();
		// ---- Buffer ----
		irc::epollret receiveToBuffer();
		irc::epollret sendFromBuffer();
		std::vector<std::string> getRawStrings();
		void putReply2Buff(const std::string&);
		void eraseBufOut();
		// ---- Get ----
		int getFD() const;
		int	getRegistrationFlags() const;
		std::string	getNick() const;
		std::string	getUserName() const;
		std::string	getRealName() const;
		std::string getHost() const;
		std::string getID() const;
		std::deque<Channel*> getChannelsList() const;
		bool getCap() const;
		bool hasQuit() const;
		bool isBufferOutFilled() const;
		bool isBufferFull(e_buffer) const;
		bool toBeKilled() const;
		std::time_t getSpamTime() const;
		irc::uint getSpamCount() const;
		// ---- Set ----
		bool setRegistrationFlags(int flags);
		void setNick(const std::string & str);
		void setUserName(const std::string & str);
		void setRealName(const std::string & str);
		void setCap(bool);
		void setQuit(bool);
		void setSpamTime(std::time_t);
		void setSpamCount(irc::uint);
		void incrementSpamCount ();
		// ---- Channels ----
		bool isChannelMember(Channel*) const;
		void addChannel(Channel*);
		void removeChannel(Channel*);

	private:
		// ----
		int _fd;
		// ----
		std::string 		 _bufIN;			 // continuous storage for input data
		std::string			 _bufOUT;			 // continuous storage for output msg
		unsigned char 		 _registrationFlags; // FIXME needs type from newer ft_irc.hpp on Channels branch
		bool				 _capRequested;		 // track if client requested CAP
		bool				 _hasQuit;			 // track if a client sent QUIT
		bool				 _toBeKilled;		 // forceful disconnect pending
		std::string			 _nick;				 // client's nick name
		std::string			 _userName;			 // client's user name
		std::string			 _realName;			 // client's real name
		const sockaddr_in& 	 _address;			 // original client IPv4 data
		std::string			 _host;				 // clients hostname (IP) as string
		std::deque<Channel*> _channels;			 // channels the client is registered to
		time_t				 _lastSpamTime;	 	 // time of last msg processed in msgs queue
		irc::uint			 _spamCount;		 // counts appends within a time
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
