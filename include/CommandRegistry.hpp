/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CommandRegistry.hpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mweghofe <mweghofe@student.42vienna.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/09 16:50:07 by mweghofe          #+#    #+#             */
/*   Updated: 2026/08/11 11:57:07 by mweghofe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef COMMANDREGISTRY_HPP
# define COMMANDREGISTRY_HPP

// -------------------------------------------------------------------------- //

# include <map>
# include <string>
# include "Command.hpp"

// -------------------------------------------------------------------------- //

class Server;
class Client;

class CommandRegistry
{
	public:
		// ----
		CommandRegistry();
		~CommandRegistry();
		// ----
		// ----
		void registerCmd(const std::string&, const Command*);
		void dispatch(Server&, Client&, const Message&);
	private:
		// ----
		std::map<const std::string, const Command*>		commands_;	// maps CMD to FUNCPTR
		// ----
		// ----
		CommandRegistry(const CommandRegistry&);
		CommandRegistry operator=(const CommandRegistry&);
};

// -------------------------------------------------------------------------- //

#endif

/*
	FROM FIRST DESIGN PLANNING STAGE

-map<string, Command*> commands

+register(Command*) : void
+find(string) : Command
+run(Client*, vector<string>, Server*) : void

*/
