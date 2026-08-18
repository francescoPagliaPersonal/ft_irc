/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CommandRegistry.hpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mweghofe <mweghofe@student.42vienna.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/09 16:50:07 by mweghofe          #+#    #+#             */
/*   Updated: 2026/08/20 00:17:59 by mweghofe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef COMMANDREGISTRY_HPP
# define COMMANDREGISTRY_HPP

// -------------------------------------------------------------------------- //

# include <map>
# include <string>
# include "Command.hpp"

// -------------------------------------------------------------------------- //

class IServerCtrl;

typedef std::string (*rfcResponse)(const Message&);

class CommandRegistry
{
	public:
		// ----
		CommandRegistry();
		~CommandRegistry();
		// ----
		void registerCmds();
		void registerCodes();
		int execute(IServerCtrl&, const Message&);
		bool handleProtocolErrors(IServerCtrl&, int, const Message&);
	private:
		// ---- maps COMMAND-NAME to a COMMAND-INSTANCE with plcy & func.ptr.
		std::map<const std::string, const Command*>	_commands;
		// ---- maps RFC ERROR&REPLY codes to a msg building function
		std::map<t_uint, rfcResponse> _rfcCodes;
		// ----
		CommandRegistry(const CommandRegistry&);
		CommandRegistry operator=(const CommandRegistry&);
};

// -------------------------------------------------------------------------- //

namespace rfc
{
	std::string badCmd(const Message&);
	std::string nickInUse(const Message&);
	std::string notRegistered(const Message&);
	std::string alreadyRegistered(const Message&);
	std::string tooFewParams(const Message&);
	std::string badPassword(const Message&);
	std::string noConnection(const Message&);
	std::string noNick(const Message&);
	std::string nickBad(const Message&);
} // end of namespace

#endif

/*
	FROM FIRST DESIGN PLANNING STAGE

-map<string, Command*> commands

+register(Command*) : void
+find(string) : Command
+run(Client*, vector<string>, IServerCtrl*) : void

*/
