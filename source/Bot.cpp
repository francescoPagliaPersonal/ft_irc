/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Bot.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mweghofe <mweghofe@student.42vienna.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/02 08:12:43 by mweghofe          #+#    #+#             */
/*   Updated: 2026/09/06 20:42:43 by mweghofe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Bot.hpp"
#include "ft_irc.hpp"

#include <cstring>
#include <cerrno>

#include <sys/socket.h>

void Bot::_registerWith(const std::string& name)
{
	std::string msg;
	
	msg.append("NICK " + name + CRLF);
	msg.append("USER " + name + " 0 * :" + name + CRLF);
	msg.append("PASS " + _pw + CRLF);

	errno = 0;
	if (::send(_fd, msg.c_str(), msg.size(), 0) < 0)
		std::cerr << "Error sending registration: " << std::strerror(errno) << std::endl;
	else
		std::cout << "Sent:\n" << msg;
}
