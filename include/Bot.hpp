/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Bot.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mweghofe <mweghofe@student.42vienna.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/02 07:22:22 by mweghofe          #+#    #+#             */
/*   Updated: 2026/09/02 08:41:48 by mweghofe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BOT_HPP
# define BOT_HPP

# include "Epoll.hpp"

# include <string>

class Bot
{
	public:
		// ----
		Bot(const unsigned int, const unsigned short, const std::string&);
		~Bot();
		// ----
		void run();
	private:
		// ----
		int _fd;
		Epoll _epoll;
		// ====
		Bot();
		Bot(const Bot&);
		Bot& operator=(const Bot&);
};

#endif
