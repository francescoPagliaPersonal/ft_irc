/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server-housekeeping.cpp                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mweghofe <mweghofe@student.42vienna.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/03 11:38:33 by mweghofe          #+#    #+#             */
/*   Updated: 2026/09/04 13:59:18 by mweghofe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Client.hpp"
#include "Server.hpp"
#include "irc.hpp"

#include <map>
#include <iostream>

void Server::_housekeeping()
{
	std::map<int, Client*>::iterator it = _clients.begin();
	
	while (it != _clients.end())
	{
		Client* client = it->second;
		std::cout << "[DEBUG] Client " << client->getNick() << " buffer IN|OUT: "
			<< client->inSize() << '|' << client->outSize() << '\n';
		it++;
		if (client->toBeKilled())
			_deleteClient(client);
	}
}
