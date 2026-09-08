/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CommandRegistry.hpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mweghofe <mweghofe@student.42vienna.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/09 16:50:07 by mweghofe          #+#    #+#             */
/*   Updated: 2026/09/08 15:42:39 by mweghofe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef COMMANDREGISTRY_HPP
# define COMMANDREGISTRY_HPP

// -------------------------------------------------------------------------- //

# include <map>
# include <string>

# include "Command.hpp"
# include "irc.hpp"

# ifdef BONUS
#  include "IBot.hpp"
# endif

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
		rfc execute(IServerCtrl&, const Message&) const;
	private:
		// ---- maps COMMAND-NAME to a COMMAND-INSTANCE with plcy & func.ptr.
		std::map<const std::string, const Command*>	_commands;
		// ----
		// ----
		CommandRegistry(const CommandRegistry&);
		CommandRegistry operator=(const CommandRegistry&);

# ifdef BONUS
	public:
		rfc execute(IBot&, const Message&) const;
# endif
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
