/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server-procInputBuffer.cpp                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fpaglia <fpaglia@student.42vienna.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/12 10:05:02 by fpaglia           #+#    #+#             */
/*   Updated: 2026/08/12 10:05:16 by fpaglia          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Message.hpp"
#include "Server.hpp"
#include <deque>

void Server::_removeMsgsFromSuspicious(Client *client)
{
	std::deque<Message>::iterator it = _msgsQueue.begin();
	while (it != _msgsQueue.end())
    {
        if (it->sender == client)
            it = _msgsQueue.erase(it); 
        else
            ++it;
    }
}

bool Server::_processInputBuffer(Client *client)
{
	std::vector<std::string> rawStrs = client->getRawStrings();
	for (size_t i = 0; i < rawStrs.size(); ++i)
	{
		if (rawStrs[i].size() > MSG_MAX_LENGTH)
		{
			if (DEBUG)
			{
				std::cout 
					<< "Found a message longer than MSG_MAX_LENGTH from client: " 
					<< client->getFD() << "\n"
					<< "eliminating all the messages appended in this batch.\n" 
					<< "Closing connection now." 
					<< std::endl;
			}
			_removeMsgsFromSuspicious(client);
			return false;
		}
		Message tmp = string2Message(rawStrs[i], client);
		if (tmp.flags & MSG_HAS_COMMAND)
			_msgsQueue.push_back(tmp);
	}
	if (DEBUG)
	{
		std::cout 
			<< "Processing input buffer for client: " << client->getFD() << "\n"
			<< "new raw strings added: " << rawStrs.size() << "\n"
			<< "new messages added: " << _msgsQueue.size()
			<< std::endl;
	}
	return true;
}
