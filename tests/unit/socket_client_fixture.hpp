/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   socket_client_fixture.hpp                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mweghofe <mweghofe@student.42vienna.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/31 11:00:00 by mweghofe          #+#    #+#             */
/*   Updated: 2026/08/31 11:00:00 by mweghofe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SOCKET_CLIENT_FIXTURE_HPP
# define SOCKET_CLIENT_FIXTURE_HPP

# include "Client.hpp"
# include "harness.hpp"
# include "irc.hpp"

# include <cerrno>
# include <cstring>
# include <string>
# include <vector>

# include <netinet/in.h>
# include <sys/socket.h>
# include <unistd.h>

struct SocketTestClient
{
	sockaddr_in	addr;
	int			peer;
	Client		*client;

	SocketTestClient()
		: addr()
		, peer(-1)
		, client(0)
	{
		int	fds[2];

		if (socketpair(AF_UNIX, SOCK_STREAM, 0, fds) != 0)
		{
			CHECK(false);
			return ;
		}
		peer = fds[1];
		client = new Client(fds[0], addr);
	}

	~SocketTestClient()
	{
		delete client;
		if (peer > -1)
			::close(peer);
	}

	void	feed(const std::string &chunk) const
	{
		ssize_t	ret;
		size_t	off;

		off = 0;
		while (off < chunk.size())
		{
			ret = ::write(peer, chunk.c_str() + off, chunk.size() - off);
			if (ret < 0)
			{
				CHECK(false);
				return ;
			}
			off += static_cast<size_t>(ret);
		}
	}

	std::vector<std::string>	feedRecvSplit(const std::string &chunk)
	{
		feed(chunk);
		CHECK_EQ(client->receiveToBuffer(), irc::RET_PARSEINPUT);
		return (client->getRawStrings());
	}
};

#endif
