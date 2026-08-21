/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server-Broadcast.cpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fpaglia <fpaglia@student.42vienna.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/19 17:36:18 by fpaglia           #+#    #+#             */
/*   Updated: 2026/08/21 16:23:08 by fpaglia          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"
#include "Channel.hpp"
#include "ft_irc.hpp"
#include <cstddef>
#include <vector>

void Server::broadcast(const std::string& reply, std::map<Client*, t_uint8> channelMembers, Client* client)
{
	std::map<Client*, t_uint8>::const_iterator it = channelMembers.begin();
	for (; it != channelMembers.end(); ++it)
	{
		if (it->first != client)
			sendMessage(*it->first, reply);
	}
}
