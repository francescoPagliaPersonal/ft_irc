/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mweghofe <mweghofe@student.42vienna.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/09 14:24:23 by mweghofe          #+#    #+#             */
/*   Updated: 2026/08/12 14:21:28 by mweghofe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_irc.hpp"
#include "args.hpp"
#include "Server.hpp"

int main(int argc, char** argv)
{
	if (argc != 3)
	{
		std::cout << "Usage: ./ircserv <port> <password>" << std::endl;
		return (1);
	}
	try
	{
		Server server(arg2port(argv[1]), arg2password(argv[2]));
		server.run();
	}
	catch (const std::exception& e)
	{
		std::cerr << e.what() << std::endl;
		return (2);
	}
	return (0);
}
