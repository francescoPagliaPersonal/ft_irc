/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Command.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mweghofe <mweghofe@student.42vienna.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/09 17:15:32 by mweghofe          #+#    #+#             */
/*   Updated: 2026/08/16 11:24:12 by mweghofe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef COMMAND_HPP
# define COMMAND_HPP

// -------------------------------------------------------------------------- //

# include <string>
# include <vector>
# include "Message.hpp"

// -------------------------------------------------------------------------- //

class IServerCtrl;
class IPolicy;

typedef int (*command)(IServerCtrl&, const Message&);

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
		int execute(IServerCtrl&, const Message&) const; // TODO func type used?
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

int cmd_pass(IServerCtrl&, const Message&);
int cmd_nick(IServerCtrl&, const Message&);
int cmd_user(IServerCtrl&, const Message&);
int cmd_cap(IServerCtrl&, const Message&);

#endif

/*
	FROM FIRST DESIGN PLANNING STAGE
	
-vector<IPlcy*> policies
-string name
-pointerFunction cmdFunction

+addPolicy(IPlcy*) : void
+execute(Client*, vector<string>, IServerCtrl*) : void

*/
