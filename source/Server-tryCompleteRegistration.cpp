/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server-tryCompleteRegistration.cpp                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mweghofe <mweghofe@student.42vienna.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/19 13:19:17 by mweghofe          #+#    #+#             */
/*   Updated: 2026/08/19 13:46:10 by mweghofe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"
#include <sstream>
// #include <arpa/inet.h> // for the commented out block below

namespace {

void build00line(std::stringstream& ss, uint numeric, const std::string& nick)
{
	ss  << ":CoolServ" << " 00" << numeric << ' ' << nick;
}

void buildWelcomeMessage(std::stringstream& ss, Client& client)
{
	const std::string nick(client.getNick());
	build00line(ss, rfc::WELCOME, nick);
	ss	<< " :Welcome to the 42 IRC " << client.getNick()
		<< "!" << client.getUserName() << CRLF;
		// << "@" << inet_ntoa(client.addr().sin_addr) << "\r\n";
	build00line(ss, rfc::YOURHOST, nick);
	ss	<< " :You host is CoolServ, calmly serving you ft_irc." << CRLF;
	build00line(ss, rfc::CREATED, nick);
	ss	<< " :The server was started on <timestamp>." << CRLF;
		// this is the mode info line: o is SERVER operator, itkol are channel MODE
	build00line(ss, rfc::MYINFO, nick);
	ss	<< " CoolServ ft_irc-v202608 o itkol" << CRLF;
}

void buildMotdLine(std::stringstream& ss, const std::string& nick, const std::string& msg)
{
	ss << ":CoolServ " << rfc::MOTD << ' ' << nick << ' ' << msg << CRLF;
}

void buildMessageOfTheDay(std::stringstream& ss, Client& client)
{
	const std::string nick(client.getNick());
	ss	<< ":CoolServ " << rfc::MOTDSTART << ' ' << nick
		<< " :CoolServ presents daily wisdom" << CRLF;
	// TODO consider ISUPPORT and LUSER (stats of inspircd)
	buildMotdLine(ss, nick, "+==============================================+");
	buildMotdLine(ss, nick, "|          Alle Wege führen nach Rom.          |");
	buildMotdLine(ss, nick, "+==============================================+");
	ss	<< ":CoolServ " << rfc::ENDOFMOTD << ' ' << nick
		<< " :End of MOTD" << CRLF;
}

} // end of namespace

// Send the welcome message once the client finished the registration handshake.
void Server::tryCompleteRegistration(Client& client)
{
	if (client.getRegistrationFlags() != REG_DONE)
		return ;
	std::cout << "[FD " << client.getFD() << "] User registration completed.\n";
	std::stringstream ss;
	buildWelcomeMessage(ss, client);
	buildMessageOfTheDay(ss, client);
	sendMessage(client, ss.str());
}
