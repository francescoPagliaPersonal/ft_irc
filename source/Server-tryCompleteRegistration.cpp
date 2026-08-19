/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server-tryCompleteRegistration.cpp                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mweghofe <mweghofe@student.42vienna.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/19 13:19:17 by mweghofe          #+#    #+#             */
/*   Updated: 2026/08/19 13:19:51 by mweghofe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"
#include <sstream>
// #include <arpa/inet.h> // for the commented out block below

// Send the welcome message once the client finished the registration handshake.
void Server::tryCompleteRegistration(Client& client)
{
	if (client.getRegistrationFlags() != REG_DONE)
		return ;
	std::stringstream ss;
	ss  << ":CoolServ" << " 00" << rfc::WELCOME << ' ' << client.getNick()
		<< " :Welcome to the 42 IRC " << client.getNick()
		<< "!" << client.getUserName() << CRLF;
		// << "@" << inet_ntoa(client.addr().sin_addr) << "\r\n";
	ss	<< ":CoolServ" << " 00" << rfc::YOURHOST << ' ' << client.getNick()
		<< " :You host is CoolServ, calmly serving you ft_irc." << CRLF;
	ss	<< ":CoolServ" << " 00" << rfc::CREATED << ' ' << client.getNick()
		<< " :The server was started on <timestamp>." << CRLF;
	// this is the mode info line: o is SERVER operator, itkol are channel MODE
	ss	<< ":CoolServ" << " 00" << rfc::MYINFO << ' ' << client.getNick()
		<< " CoolServ ft_irc-v202608 o itkol" << CRLF;
	std::cout << "[FD " << client.getFD() << "] User registration completed.\n";
	sendMessage(client, ss.str());
}
