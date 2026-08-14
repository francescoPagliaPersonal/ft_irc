/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_user.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mweghofe <mweghofe@student.42vienna.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/12 13:54:12 by fpaglia           #+#    #+#             */
/*   Updated: 2026/08/14 10:33:38 by mweghofe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


# include "Command.hpp"
# include "Server.hpp"
# include "Client.hpp"
#include <arpa/inet.h>
#include <sstream>
#include <sys/epoll.h>

namespace  {
	std::string welcomeMessage(Client & client)
	{
		std::stringstream ss;

		ss  << ":CoolServ 001 " << client.getNick() 
			<< " :Welcome to the IRC " 
			<< client.getNick() << "!" << client.getUserName() << CRLF;
			// << "@" << inet_ntoa(client.addr().sin_addr) << "\r\n";
		return ss.str();
	}
}
int cmd_user(Server & srv, const Message & msg)
{
	std::cout << "executing" << msg.command << std::endl;

	// TODO: this is just a quick proof of concept more complex UserName evaluation 
	// should be carried out (against the whole server).
	Client *client = msg.sender;
	if (client == NULL)
		return 1; // ERR_HANGUP??

	client->setUserName(msg.params[0]);
	client->setRealName(msg.trailing);
	std::cout << "UserName registration successfull." << std::endl;
	client->setRegistrationFlags(REG_USER);
	if (client->getRegistrationFlags() == REG_DONE)
	{
		std::cout << "User registration completed." << std::endl;
		client->putReply2Buff(srv, welcomeMessage(*client));
	}
	
	return (0);
}
