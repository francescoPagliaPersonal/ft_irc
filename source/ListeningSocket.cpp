/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ListeningSocket.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mweghofe <mweghofe@student.42vienna.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/09 17:55:20 by mweghofe          #+#    #+#             */
/*   Updated: 2026/08/09 21:42:59 by mweghofe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ListeningSocket.hpp"

// -------------------------------------------------------------------------- //
// CUSTOM CTOR & DTOR
// -------------------------------------------------------------------------- //

ListeningSocket::ListeningSocket(unsigned short port)
	: fd_(-1)
{
	(void) port;
	// TODO fill me up
}

ListeningSocket::~ListeningSocket()
{}

// -------------------------------------------------------------------------- //
// OPERATION
// -------------------------------------------------------------------------- //

// -------------------------------------------------------------------------- //
// OCF
// -------------------------------------------------------------------------- //

ListeningSocket::ListeningSocket()
	: fd_(-1)
{}

ListeningSocket::ListeningSocket(const ListeningSocket& other)
	: fd_(-1)
{
	(void) other;
}

ListeningSocket ListeningSocket::operator=(const ListeningSocket& other)
{
	(void) other;
	return (*this);
}
