/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client_fixture.hpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mweghofe <mweghofe@student.42vienna.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/24 10:48:00 by mweghofe          #+#    #+#             */
/*   Updated: 2026/08/24 10:48:00 by mweghofe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENT_FIXTURE_HPP
# define CLIENT_FIXTURE_HPP

# include "Client.hpp"

# include <netinet/in.h>

struct TestClient
{
	sockaddr_in	addr;
	Client		client;

	TestClient()
		: addr()
		, client(-1, addr)
	{}
};

#endif
