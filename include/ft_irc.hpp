/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_irc.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mweghofe <mweghofe@student.42vienna.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/09 14:25:38 by mweghofe          #+#    #+#             */
/*   Updated: 2026/08/20 11:35:45 by mweghofe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_IRC_HPP
# define FT_IRC_HPP

// -------------------------------------------------------------------------- //

# include <iostream>
# include <stdexcept>

// -------------------------------------------------------------------------- //

# define MAX_CLIENTS 50000
# define MAX_EVENTS 32
# define TIMEOUT 1000
# define CRLF "\r\n"
# define MSG_MAX_LENGTH 512
# define MAX_NICKLEN 32

# ifndef DEBUG
#  define DEBUG 0 // TODO set this to zero later and ctl via makefile
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

enum e_pollret 
{
	RET_OK,
	RET_EMPTY,
	RET_CLOSE,
	RET_HASOUTPUT,
	RET_PARSEINPUT
};

// -------------------------------------------------------------------------- //

typedef unsigned int		t_uint;
typedef unsigned char		t_uint8;
typedef unsigned long int	t_uint32;

#endif
