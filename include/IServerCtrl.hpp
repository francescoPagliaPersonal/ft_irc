/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   IServerCtrl.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mweghofe <mweghofe@student.42vienna.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/14 19:25:52 by mweghofe          #+#    #+#             */
/*   Updated: 2026/09/04 07:00:09 by mweghofe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ISERVERCTRL_HPP
# define ISERVERCTRL_HPP

// -------------------------------------------------------------------------- //

# include <string>
# include <vector>
# include <map>
# include <set>

# include "irc.hpp"

// -------------------------------------------------------------------------- //

class Client;
class Channel;

class IServerCtrl
{
public:
	// ----
	virtual ~IServerCtrl() {}
	// ---- Get Server Info
	virtual std::string getPassword() const = 0;
	virtual std::string getStartTime() const = 0;
	// virtual std::string getServerName() const = 0; // or a DEFINE
	// ---- Registration
	virtual void tryCompleteRegistration(Client*) const = 0;
	// ---- Find By Name
	virtual Client* findClientByNick(const std::string&) const = 0;
	// ---- Sending Messages To Clients
	virtual void sendMessage(Client*, const std::string&) const = 0;
	virtual void broadcast(Channel*, Client *, const std::string&) const = 0;
	virtual void broadcast(Channel*, const std::string&) const = 0;
	virtual void broadcast(std::set<Client*>&, const std::string&) const = 0;
	// ---- Channel Manipulation
	virtual rfc addToChannel(Client*, const std::string&, const std::string&) = 0;
	virtual void removeFromChannel(Client*, const std::string&, const std::string&) = 0;
	virtual Channel * getChannelByTitle(std::string) const = 0;
	// ---- Operation
	// virtual void disconnectClient(Client&, const std::string&) = 0;
};

#endif
