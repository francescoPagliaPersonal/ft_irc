/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_cmd_helper.hpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mweghofe <mweghofe@student.42vienna.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/31 10:33:00 by mweghofe          #+#    #+#             */
/*   Updated: 2026/08/31 10:33:00 by mweghofe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TEST_CMD_HELPER_HPP
# define TEST_CMD_HELPER_HPP

# include "Client.hpp"
# include "client_fixture.hpp"
# include "fake_server.hpp"

# include <string>

inline std::string	lastTo(const FakeServer &srv, Client *c)
{
	if (srv.sent.empty())
		return (std::string());
	for (size_t i = srv.sent.size(); i > 0; --i)
	{
		if (srv.sent[i - 1].first == c)
			return (srv.sent[i - 1].second);
	}
	return (std::string());
}

inline bool	sentContains(const FakeServer &srv, Client *c,
			const std::string &needle)
{
	for (size_t i = 0; i < srv.sent.size(); ++i)
	{
		if (srv.sent[i].first == c
			&& srv.sent[i].second.find(needle) != std::string::npos)
			return (true);
	}
	return (false);
}

inline void	registerClient(FakeServer &srv, TestClient &tc,
				const std::string &nick)
{
	tc.client.setNick(nick);
	tc.client.setUserName("user");
	tc.client.setRegistrationFlags(REG_DONE);
	srv.nicks[nick] = &tc.client;
}

#endif
