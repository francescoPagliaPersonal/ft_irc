/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server-tryCompleteRegistration.cpp                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mweghofe <mweghofe@student.42vienna.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/19 13:19:17 by mweghofe          #+#    #+#             */
/*   Updated: 2026/09/16 14:59:24 by mweghofe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_irc.hpp"
#include "Client.hpp"
#include "Server.hpp"
#include "Response.hpp"
#include <sstream>
// #include <arpa/inet.h> // for the commented out block below

namespace {



void build00line(std::stringstream& ss, uint numeric, const std::string& nick)
{
	std::string srvName = Response::getServerName();
	ss  << ":" + srvName << " 00" << numeric << ' ' << nick;
}

void buildWelcomeMessage(std::stringstream& ss, Client* client, const std::string& time)
{
	std::string srvName = Response::getServerName();
	const std::string nick(client->getNick());
	build00line(ss, irc::WELCOME, nick);
	ss	<< " :Welcome to the 42 IRC " << client->getNick()
		<< "!" << client->getUserName() << CRLF;
		// << "@" << inet_ntoa(client->addr().sin_addr) << "\r\n";
	build00line(ss, irc::YOURHOST, nick);
	ss	<< " :You host is " + srvName +", calmly serving you ft_irc." << CRLF;
	build00line(ss, irc::CREATED, nick);
	ss	<< " :The server was started on " << time  << '.' << CRLF;
		// this is the mode info line: o is SERVER operator, itkol are channel MODE
	build00line(ss, irc::MYINFO, nick);
	ss	<< " " + srvName + " ft_irc-v202608 itkol" << CRLF;
}

void buildMotdLine(std::stringstream& ss, const std::string& nick, const std::string& msg)
{
	std::string srvName = Response::getServerName();
	ss << ":" + srvName + " " << irc::MOTD << ' ' << nick << ' ' << msg << CRLF;
}

void buildMessageOfTheDay(std::stringstream& ss, Client* client)
{
	std::string srvName = Response::getServerName();
	const std::string nick(client->getNick());
	ss	<< ":" + srvName + " " << irc::MOTDSTART << ' ' << nick
		<< " :" + srvName + " presents daily wisdom" << CRLF;
	buildMotdLine(ss, nick, "+==============================================+");
	buildMotdLine(ss, nick, "|          Alle Wege führen nach Rom.          |");
	buildMotdLine(ss, nick, "+==============================================+");
	ss	<< ":" + srvName + " " << irc::ENDOFMOTD << ' ' << nick
		<< " :End of MOTD" << CRLF;
}

} // end of namespace

// Send the welcome message once the client finished the registration handshake.
void Server::tryCompleteRegistration(Client* client) const
{
	if (client->getRegistrationFlags() != REG_DONE)
		return ;
	std::cout << "[FD " << client->getFD() << "] User registration completed.\n";
	std::stringstream ss;
	buildWelcomeMessage(ss, client, _getStartTimeString());
	buildMessageOfTheDay(ss, client);
	sendMessage(client, ss.str());
}
