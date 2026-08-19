/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fpaglia <fpaglia@student.42vienna.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/13 10:19:35 by mweghofe          #+#    #+#             */
/*   Updated: 2026/08/19 18:34:12 by fpaglia          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CHANNEL_HPP
# define CHANNEL_HPP

// -------------------------------------------------------------------------- //

# include <string>
# include <map>
#include <vector>

# include "ft_irc.hpp"

// -------------------------------------------------------------------------- //

# define MAX_CHANNELUSERS 100
// TODO need the right bit values
# define CH_INVITE
# define CH_TOPIC
# define CH_PASSWORD
# define CH_OPERATOR

// -------------------------------------------------------------------------- //

typedef t_uint8 bitMask;
class Client;

class Channel
{
	public:
		// ----
		Channel(const std::string&, const std::string&);
		~Channel();
		// ----
		// ---- operation
		void addClient(Client*);
		void removeClient(Client*);
		bool isEmpty() const;
		// ---- get
		std::string getTitle() const;		// used in debug & test functions
		std::string getTopic() const;
		std::string getPassword() const;
		t_uint getLimit() const;
		std::map<Client*, bitMask>	getClientMap() const;
		
		// ---- set
		void setTopic(const std::string&);
		void setPassword(const std::string&);
		void setLimit(t_uint);
	private:
		// ----
		bitMask 							_modes;
		t_uint								_userLimit;
		const std::string					_title; // RENAME cmd not required
		std::string							_topic;
		std::string							_password;
		std::map<Client*, bitMask>			_members;
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
