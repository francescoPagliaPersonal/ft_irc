/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Command.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mweghofe <mweghofe@student.42vienna.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/09 17:15:32 by mweghofe          #+#    #+#             */
/*   Updated: 2026/08/30 10:13:14 by mweghofe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef COMMAND_HPP
# define COMMAND_HPP

// -------------------------------------------------------------------------- //

# include <string>
# include <vector>
# include <deque>

# include "Message.hpp"
# include "irc.hpp"

// -------------------------------------------------------------------------- //

# define MODES "itkol"

// -------------------------------------------------------------------------- //

class IServerCtrl;
class IPolicy;

typedef rfc (*command)(IServerCtrl&, const Message&);

class Command
{
	public:
		// ----
		Command(const std::string&, command);
		~Command();
		// ----
		struct Data
		{
			IServerCtrl& srv;
			const Message& msg;
			Client* client;
			Channel* channel;
			std::deque<std::string> modeChOPadd;
			std::deque<std::string> modeChOPrem;
			// ----
			Data(IServerCtrl&, const Message&, Client*);
		};
		// ----
		std::string getName() const;
		void addPolicy(IPolicy*);
		rfc execute(IServerCtrl&, const Message&) const;
	private:
		// ----
		std::string	_name;				 // name of the command
		command 	_func;				 // function handler for the command
		std::vector<IPolicy*> _policies; // active policies per command
		// ----
		// ----
		Command();
		Command(const Command&);
		Command operator=(const Command&);
};

// -------------------------------------------------------------------------- //

rfc cmd_pass(IServerCtrl&, const Message&);
rfc cmd_nick(IServerCtrl&, const Message&);
rfc cmd_user(IServerCtrl&, const Message&);
rfc cmd_cap(IServerCtrl&, const Message&);
rfc cmd_ping(IServerCtrl&, const Message&);
rfc cmd_join(IServerCtrl&, const Message&);
rfc cmd_privmsg(IServerCtrl&, const Message&);
rfc cmd_invite(IServerCtrl&, const Message&);
rfc cmd_mode(IServerCtrl&, const Message&);

#endif

/*
	FROM FIRST DESIGN PLANNING STAGE
	
-vector<IPlcy*> policies
-string name
-pointerFunction cmdFunction

+addPolicy(IPlcy*) : void
+execute(Client*, vector<string>, IServerCtrl*) : void

*/
