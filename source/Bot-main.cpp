/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Bot-main.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mweghofe <mweghofe@student.42vienna.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/02 08:11:16 by mweghofe          #+#    #+#             */
/*   Updated: 2026/09/02 09:01:58 by mweghofe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Bot.hpp"
#include "args.hpp"

#include <iostream>

int main (int argc, char** argv)
{
	if (argc != 4)
	{
		std::cout << "Usage: ./bot <serverip> <port> <password>" << std::endl;
		return (1);
	}
	try
	{
		Bot bot(arg2ip(argv[1]), arg2port(argv[2]), arg2password(argv[3]));
		bot.run();
	}
	catch (const std::exception& e)
	{
		std::cerr << e.what() << std::endl;
		return (2);
	}
	return (0);
}
