/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fake_server.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mweghofe <mweghofe@student.42vienna.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/24 10:48:00 by mweghofe          #+#    #+#             */
/*   Updated: 2026/08/31 09:40:00 by mweghofe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FAKE_SERVER_HPP
# define FAKE_SERVER_HPP

# include "Client.hpp"
# include "IServerCtrl.hpp"
# include "Response.hpp"
# include "irc.hpp"

# include <map>
# include <string>
# include <utility>
# include <vector>

class FakeServer : public IServerCtrl
{
public:
	std::string												password;
	std::map<std::string, Client*>							nicks;
	mutable std::vector<std::pair<Client*, std::string> >	sent;
	mutable int												completeCalls;

	FakeServer()
		: completeCalls(0)
	{
		Response::init("CoolServ");
	}

	std::string	getPassword() const
	{
		return (password);
	}

	void	tryCompleteRegistration(Client *c) const
	{
		if (!c || c->getRegistrationFlags() != REG_DONE)
			return ;
		++completeCalls;
	}

	Client	*findClientByNick(const std::string &n) const
	{
		std::map<std::string, Client*>::const_iterator	it;

		it = nicks.find(n);
		if (it == nicks.end())
			return (0);
		return (it->second);
	}

	void	sendMessage(Client *c, const std::string &s) const
	{
		sent.push_back(std::make_pair(c, s));
	}

	void	broadcast(Channel *, Client *, const std::string &) const
	{}

	void	broadcast(Channel *, const std::string &) const
	{}

	rfc	addToChannel(Client *, const std::string &, const std::string &)
	{
		return (irc::OK);
	}

	void	removeFromChannel(Client *, const std::string &, const std::string &)
	{}

	Channel	*getChannelByTitle(std::string) const
	{
		return (0);
	}
};

#endif
