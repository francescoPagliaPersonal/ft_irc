/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server-procInputBuffer.cpp                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mweghofe <mweghofe@student.42vienna.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/12 10:05:02 by fpaglia           #+#    #+#             */
/*   Updated: 2026/08/16 18:59:24 by mweghofe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Message.hpp"
#include "Server.hpp"
#include <deque>

namespace {

	void printMessage(const Message & msgs, size_t i)
	{
		if (i == 1)
			std::cout << std::string(10, '-') <<std::endl;
		std::cout
			<< "Message #" << i  << '\n'
			<< "Arg count: " << argCount(msgs)  << '\n'
			<< "Prefix   : {" << msgs.prefix << "}" << '\n'
			<< "Command  : {" << msgs.command << "}" << '\n';
		for (size_t j = 0; j < msgs.params.size(); ++j)
		{
			std::cout << "Params   : {" << msgs.params[j] << "}\n";
		}
		std::cout 
			<< "Trailing : {" << msgs.trailing << "}\n";
		std::cout << std::string(10, '-') <<std::endl;
	}
}

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
					<< client->getFD() << '\n'
					<< "eliminating all the messages appended in this batch.\n" 
					<< "Closing connection now." 
					<< std::endl;
			}
			_removeMsgsFromSuspicious(client);
			return false;
		}
		Message tmp = string2Message(rawStrs[i], client);
		if (tmp.flags & MSG_HAS_COMMAND)
		{
			_msgsQueue.push_back(tmp);
			if (DEBUG)
				printMessage(tmp, i + 1);
		}
	}
	if (DEBUG)
	{
		std::cout 
			<< "[FD " << client->getFD() << "] Input buffer processed: "
			<< "added " << rawStrs.size() << " strings for a total of "
			<< _msgsQueue.size() << " messages\n";
	}
	return true;
}
