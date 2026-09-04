/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server-tryCompleteRegistration.cpp                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mweghofe <mweghofe@student.42vienna.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/19 13:19:17 by mweghofe          #+#    #+#             */
/*   Updated: 2026/09/04 07:18:28 by mweghofe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_irc.hpp"
#include "Client.hpp"
#include "Server.hpp"

#include <sstream>
// #include <arpa/inet.h> // for the commented out block below

namespace {

void build00line(std::stringstream& ss, uint numeric, const std::string& nick)
{
	ss  << ":CoolServ" << " 00" << numeric << ' ' << nick;
}

void buildWelcomeMessage(std::stringstream& ss, Client* client, const std::string& time)
{
	const std::string nick(client->getNick());
	build00line(ss, irc::WELCOME, nick);
	ss	<< " :Welcome to the 42 IRC " << client->getNick()
		<< "!" << client->getUserName() << CRLF;
		// << "@" << inet_ntoa(client->addr().sin_addr) << "\r\n";
	build00line(ss, irc::YOURHOST, nick);
	ss	<< " :You host is CoolServ, calmly serving you ft_irc." << CRLF;
	build00line(ss, irc::CREATED, nick);
	ss	<< " :The server was started on " << time  << '.' << CRLF;
		// this is the mode info line: o is SERVER operator, itkol are channel MODE
	build00line(ss, irc::MYINFO, nick);
	ss	<< " CoolServ ft_irc-v202608 o itkol" << CRLF;
}

void buildMotdLine(std::stringstream& ss, const std::string& nick, const std::string& msg)
{
	ss << ":CoolServ " << irc::MOTD << ' ' << nick << ' ' << msg << CRLF;
}

void buildMessageOfTheDay(std::stringstream& ss, Client* client)
{
	const std::string nick(client->getNick());
	ss	<< ":CoolServ " << irc::MOTDSTART << ' ' << nick
		<< " :CoolServ presents daily wisdom" << CRLF;
	// TODO consider ISUPPORT and LUSER (stats of inspircd)
	buildMotdLine(ss, nick, "+==============================================+");
	buildMotdLine(ss, nick, "|          Alle Wege führen nach Rom.          |");
	buildMotdLine(ss, nick, "+==============================================+");
	ss	<< ":CoolServ " << irc::ENDOFMOTD << ' ' << nick
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
