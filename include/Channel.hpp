/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mweghofe <mweghofe@student.42vienna.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/13 10:19:35 by mweghofe          #+#    #+#             */
/*   Updated: 2026/08/27 13:06:07 by mweghofe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CHANNEL_HPP
# define CHANNEL_HPP

// -------------------------------------------------------------------------- //

# include "irc.hpp"

# include <deque>
# include <string>
# include <map>
# include <vector>

// -------------------------------------------------------------------------- //

/*
	MODE - Change the channel’s mode:
	· i: Set/remove Invite-only channel
	· t: Set/remove the restrictions of the TOPIC command to channel
	operators
	· k: Set/remove the channel key (password)
	· o: Give/take channel operator privilege
	· l: Set/remove the user limit to channel
*/
enum e_chMask
{
	CH_INVITE = 1 << 0,
	CH_TOPIC = 1 << 1,
	CH_PASSWORD = 1 << 2,
	CH_LIMIT = 1 << 3,
	CH_LOCAL = 1 << 4
};

enum e_usrMask
{
	US_BASIC = 0,
	US_OPERATOR = 1 << 1,
	US_FOUNDER = 1 << 2,
	US_INVISIBLE = 1 << 3
};

// -------------------------------------------------------------------------- //

typedef irc::uint8 bitMask;
class Client;

class Channel
{
	public:
		// ----
		Channel(const std::string&, const std::string&);
		~Channel();
		// ----
		// ---- operation
		void addClient(Client*, bitMask);
		void removeClient(Client*);
		bool isEmpty() const;
		// ---- get
		std::string getTitle() const;		// used in debug & test functions
		std::string getTopic() const;
		std::string getPassword() const;
		irc::uint 	getLimit() const;
		bitMask		getModes() const;
		std::map<Client*, bitMask>	getMembersMap() const;
		
		// ---- set
		void setTopic(const std::string&);
		void setPassword(const std::string&);
		void setLimit(irc::uint);
		void setInvite();

		static bool isTitleCompliant(const std::string& channel);
		static std::string title2key(std::string);

		bool isMember(Client *);
		bool passwordMatch(const std::string & pw);
		bool belowChannelLimit();
		bool joinGranted(Client *);
		
	private:
		// ----
		bitMask 							_modes;
		irc::uint							_userLimit;
		const std::string					_title; // RENAME cmd not required
		std::string							_topic;
		std::string							_password;
		std::map<Client*, bitMask>			_members;
		std::deque<Client *>				_invites;
		// ----
		// ----
		Channel();
		Channel(const Channel&);
		Channel operator=(const Channel&);
};

// -------------------------------------------------------------------------- //

#endif

/*

-bitMask modes
-uint userLimit
-string title
-string topic
-string password
-map<Client*, int> members
-vector<IPlcy*> policies

+addClient(...) : bool
+removeClient(...) : bool
+isCop(...) : bool
+getTopic() : bool
+setTopic(...) : bool
+getPassword() : bool
+setPassword(...) : bool
+getLimit() : bool
+setLimit(...) : bool
+broadcast(...) : bool

*/
