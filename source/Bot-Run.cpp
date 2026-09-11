/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Bot.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fpaglia <fpaglia@student.42vienna.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/11 17:04:38 by fpaglia           #+#    #+#             */
/*   Updated: 2026/09/11 17:20:19 by fpaglia          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Bot.hpp"
#include "ft_irc.hpp"

#include <cstring>

void Bot::run()
{
	int secDelay = 5;
	int attempt = 3;
	
	while (_keepRunning)
	{
		_fd = connectWithRetry(attempt, secDelay);
		if (_fd == -1)
		{
			secDelay = secDelay <= 640 ? secDelay *2 : secDelay;
			continue ;	
		}
		std::cout << "[bot] registered with fd: " << _fd << std::endl;

		// stay alive in epoll loop
		_epollHandler();
		_bufIN.clear();
		_bufOUT.clear();
		if (_fd != -1) {
            _epoll.del(_fd);
            ::close(_fd);
            _fd = -1;
        }
	}

}
