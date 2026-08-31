/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fake_server.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mweghofe <mweghofe@student.42vienna.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/24 10:48:00 by mweghofe          #+#    #+#             */
/*   Updated: 2026/08/31 09:08:03 by mweghofe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FAKE_SERVER_HPP
# define FAKE_SERVER_HPP

# include "Client.hpp"
# include "IServerCtrl.hpp"

# include <map>
# include <string>
# include <utility>
# include <vector>

class FakeServer : public IServerCtrl
{
public:
	std::string									password;
	std::map<std::string, Client*>				nicks;
	std::vector<std::pair<Client*, std::string> >	sent;
	int											completeCalls;

	FakeServer()
		: completeCalls(0)
	{}

	std::string	getPassword() const
	{
		return (password);
	}

	void	tryCompleteRegistration(Client &c)
	{
		if (c.getRegistrationFlags() != REG_DONE)
			return ;
		++completeCalls;
	}

	Client	*findClientByNick(const std::string &n)
	{
		std::map<std::string, Client*>::iterator	it;

		it = nicks.find(n);
		if (it == nicks.end())
			return (0);
		return (it->second);
	}

	void	sendMessage(Client &c, const std::string &s)
	{
		sent.push_back(std::make_pair(&c, s));
	}

	void	broadcastToChannel(Channel *, const std::string &, Client *)
	{}

	void	broadcastToChannel(const std::string &, const std::string &,
				Client *)
	{}

	void	addToChannel(Client *, const std::string &, const std::string &)
	{}

	void	removeFromChannel(Client *, const std::string &, const std::string &)
	{}
};

#endif
