/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server-housekeeping.cpp                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mweghofe <mweghofe@student.42vienna.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/03 11:38:33 by mweghofe          #+#    #+#             */
/*   Updated: 2026/09/04 14:18:24 by mweghofe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Client.hpp"
#include "Server.hpp"
#include "irc.hpp"
#include "ft_irc.hpp"

#include <map>

void Server::_housekeeping()
{
	static std::time_t lastTime = std::time(NULL);
	std::map<int, Client*>::iterator it = _clients.begin();
	if (std::difftime(std::time(NULL), lastTime) < HOUSEKEEPING_INTERVAL)
		return ;
	while (it != _clients.end())
	{
		Client* client = it->second;
		std::cout << "[DEBUG] Client " << client->getNick() << " buffer IN|OUT: "
			<< client->inSize() << '|' << client->outSize() << '\n';
		it++;
		if (client->toBeKilled())
			_deleteClient(client);
	}
	lastTime = std::time(NULL);
}
