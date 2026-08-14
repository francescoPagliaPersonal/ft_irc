/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Command.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mweghofe <mweghofe@student.42vienna.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/09 17:15:32 by mweghofe          #+#    #+#             */
/*   Updated: 2026/08/14 12:15:52 by mweghofe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef COMMAND_HPP
# define COMMAND_HPP

// -------------------------------------------------------------------------- //

# include <string>
# include <vector>
# include "Message.hpp"

// -------------------------------------------------------------------------- //

class Server;
class IPolicy;

typedef int (*command)(Server&, const Message&);

class Command
{
	public:
		// ----
		Command(const std::string&, command);
		~Command();
		// ----
		// ----
		std::string getName() const;
		void addPolicy(IPolicy*);
		int execute(Server&, const Message&) const; // TODO func type used?
	private:
		// ----
		std::string	_name;		// name of the command
		command 	_func;		// function handler for the command
		std::vector<IPolicy*> _policies;
		// ----
		// ----
		Command();
		Command(const Command&);
		Command operator=(const Command&);
};

// -------------------------------------------------------------------------- //

int cmd_pass(Server&, const Message&);
int cmd_nick(Server&, const Message&);
int cmd_user(Server&, const Message&);
int cmd_cap(Server&, const Message&);

#endif

/*
	FROM FIRST DESIGN PLANNING STAGE
	
-vector<IPlcy*> policies
-string name
-pointerFunction cmdFunction

+addPolicy(IPlcy*) : void
+execute(Client*, vector<string>, Server*) : void

*/
