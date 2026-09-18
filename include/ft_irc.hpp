/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_irc.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mweghofe <mweghofe@student.42vienna.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/09 14:25:38 by mweghofe          #+#    #+#             */
/*   Updated: 2026/09/18 11:43:00 by mweghofe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_IRC_HPP
# define FT_IRC_HPP

// -------------------------------------------------------------------------- //

# include <iostream>
# include <stdexcept>

// -------------------------------------------------------------------------- //
/* We consider to resever in the program at least 5 FDs following this criteria:
 * 0 1 2 stdio
 * 3 server listener
 * 4 epoll instance
 */
# define SERVER_NAME "irc.CoolServ.42"
# define RESERVED_FDS 5
# define MIN_CLIENTS 5
# define MAX_CLIENTS 50000
# define MAX_EVENTS 32
# define TIMEOUT 2000
# define CRLF "\r\n"
# define COL_GREEN "\033[32m"
# define COL_CYAN "\033[36m"
# define COL_RESET "\033[0m"
# define COL_GRAY "\033[90m"
# define MSG_MAX_LENGTH 512
# define MIN_TRAIL_LENGTH 32
# define MAX_NICKLEN 32
# define MAX_CHANLEN 32
# define MIN_CHANLEN 4
# define MAX_CHANJOIN 3
# define MAX_TOPICLEN 300
# define MAX_CHANNELUSERS 500
# define MAX_CLIENT_ON_IP 5
# define SPAM_THRESHOLD_MSGS 42
# define SPAM_TRHESHOLD_TIME 2.0
# define INTERVAL_PING 90
# define MAX_UNANSWERED_PING 1

# ifndef DEBUG
#  define DEBUG 0
# endif

// -------------------------------------------------------------------------- //

namespace debug
{
	enum e_debug
	{
		LOGSONLY = 0,
		BASIC,
		DETAILED
	};
}

// -------------------------------------------------------------------------- //

#endif
