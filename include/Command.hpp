/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Command.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mweghofe <mweghofe@student.42vienna.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/09 17:15:32 by mweghofe          #+#    #+#             */
/*   Updated: 2026/09/15 16:01:17 by mweghofe         ###   ########.fr       */
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

# ifdef BONUS
#  include "IBot.hpp"
# endif

// -------------------------------------------------------------------------- //

# define MODES "itkol"

// -------------------------------------------------------------------------- //

class IServerCtrl;
class IPolicy;

typedef rfc (*command)(IServerCtrl&, const Message&);

# ifdef BONUS
typedef void (*botcmd)(IBot&, const Message&, std::vector<std::string>&);
#endif

class Command
{
	public:
		// ----
		~Command();
		// ----
		struct Data
		{
			IServerCtrl& srv;
			const Message& msg;
			Client* client;
			Channel* channel;
			irc::uint32 prevLimit;
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
		std::vector<IPolicy*> _policies; // active policies per command
		// ----
		// ----
		Command();
		Command(const Command&);
		Command operator=(const Command&);

# ifndef BONUS
	public:
		Command(const std::string&, command);
	private:
		command 	_func;				 // function handler for the command
#  else
	public:
		Command(const std::string&, botcmd);
		void execute(IBot&, const Message&, std::vector<std::string>&) const;
	private:
		botcmd		_func;				// function handler for the bot command
# endif
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
rfc cmd_quit(IServerCtrl&, const Message&);
rfc cmd_topic(IServerCtrl&, const Message&);
rfc cmd_kick(IServerCtrl&, const Message&);
rfc cmd_part(IServerCtrl&, const Message&);
rfc cmd_pong(IServerCtrl&, const Message&);
rfc cmd_who(IServerCtrl&, const Message&);

# ifdef BONUS

void bot_help(IBot&, const Message&, std::vector<std::string>&);
void bot_spam(IBot&, const Message&, std::vector<std::string>&);
void bot_mirror(IBot&, const Message&, std::vector<std::string>&);
void bot_quote(IBot&, const Message&, std::vector<std::string>&);

# endif

#endif

/*
	FROM FIRST DESIGN PLANNING STAGE
	
-vector<IPlcy*> policies
-string name
-pointerFunction cmdFunction

+addPolicy(IPlcy*) : void
+execute(Client*, vector<string>, IServerCtrl*) : void

*/
