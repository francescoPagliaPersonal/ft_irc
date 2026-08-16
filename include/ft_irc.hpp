/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_irc.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mweghofe <mweghofe@student.42vienna.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/09 14:25:38 by mweghofe          #+#    #+#             */
/*   Updated: 2026/08/16 15:01:23 by mweghofe         ###   ########.fr       */
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
# ifndef DEBUG
#  define DEBUG 1 // TODO set this to zero later and ctl via makefile
# endif

// -------------------------------------------------------------------------- //

namespace rfc
{
	enum e_rfc
	{
		OK,
		BADCMD = 421,
		NICKBAD = 432,
		NICKINUSE,
		FEWPARAMS = 461,
		ALREADYREG,
		BADPASS = 464,
		NOCONN = 42001,
		MANYPARAMS
	};
} // end namespace rfc

// -------------------------------------------------------------------------- //

enum e_pollret 
{
	RET_OK,
	RET_EMPTY,
	RET_CLOSE,
	RET_HASOUTPUT,
	RET_PARSEINPUT
};

#endif
