/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CommandRegistry.hpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mweghofe <mweghofe@student.42vienna.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/09 16:50:07 by mweghofe          #+#    #+#             */
/*   Updated: 2026/08/24 18:00:45 by mweghofe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef COMMANDREGISTRY_HPP
# define COMMANDREGISTRY_HPP

// -------------------------------------------------------------------------- //

# include <map>
# include <string>
# include "Command.hpp"
# include "irc.hpp"

// -------------------------------------------------------------------------- //

class IServerCtrl;

class CommandRegistry
{
	public:
		// ----
		CommandRegistry();
		~CommandRegistry();
		// ----
		// ----
		void registerCmds();
		rfc execute(IServerCtrl&, const Message&);
		bool handleProtocolErrors(IServerCtrl&, int, const Message&);
	private:
		// ---- maps COMMAND-NAME to a COMMAND-INSTANCE with plcy & func.ptr.
		std::map<const std::string, const Command*>	_commands;
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
+run(Client*, vector<string>, IServerCtrl*) : void

*/
