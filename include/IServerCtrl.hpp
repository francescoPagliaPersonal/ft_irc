/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   IServerCtrl.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mweghofe <mweghofe@student.42vienna.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/14 19:25:52 by mweghofe          #+#    #+#             */
/*   Updated: 2026/08/16 16:01:49 by mweghofe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ISERVERCTRL_HPP
# define ISERVERCTRL_HPP

#include <string>

class Client;
// class Channel;

class IServerCtrl
{
public:
	// ----
	virtual ~IServerCtrl() {}
	// ---- Get Server Info
	virtual std::string getPassword() const = 0;
	// virtual std::string getServerName() const = 0; // or a DEFINE
	// ---- Registration
	virtual void tryCompleteRegistration(Client&) = 0;
	// ---- Find By Name
	virtual Client* findClientByNick(const std::string&) = 0;
	// virtual Channel* findChannel(const std::string&) = 0;
	// ---- Sending Messages To Clients
	virtual void sendMessage(Client&, const std::string&) = 0;
	// virtual void broadcastToChannel(const std::string& ch, const std::string& msg, Client* = 0) = 0;
	// ---- Channel Manipulation
	// virtual Channel* getOrCreateChannel(const std::string&) = 0;
	// virtual void addToChannel(Client&, Channel&) = 0;
	// virtual void removeFromChannel(Client&, const std::string&, const std::string&) = 0;
	// ---- Operation
	// virtual void disconnectClient(Client&, const std::string&) = 0;
};

#endif
