/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Command.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mweghofe <mweghofe@student.42vienna.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/09 17:15:32 by mweghofe          #+#    #+#             */
/*   Updated: 2026/08/12 14:18:58 by mweghofe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef COMMAND_HPP
# define COMMAND_HPP

// -------------------------------------------------------------------------- //

# include <string>
# include "Message.hpp"

// -------------------------------------------------------------------------- //

class Server;
class Client;

typedef void (*command)(Server&, Client&, const Message&);

class Command
{
	public:
		// ----
		Command(const std::string&, command);
		~Command();
		// ----
		// ----
		void getName() const;
		void addPolicy();
		void execute(Server&, Client&, const Message&);
	private:
		// ----
		std::string	_name;		// name of the command
		command 	_func;		// function handler for the command
		// ----
		// ----
		Command();
		Command(const Command&);
		Command operator=(const Command&);
};

// -------------------------------------------------------------------------- //

#endif

/*
	FROM FIRST DESIGN PLANNING STAGE
	
-vector<IPlcy*> policies
-string name
-pointerFunction cmdFunction

+addPolicy(IPlcy*) : void
+execute(Client*, vector<string>, Server*) : void

*/
