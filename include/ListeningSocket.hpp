/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ListeningSocket.hpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mweghofe <mweghofe@student.42vienna.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/09 16:19:04 by mweghofe          #+#    #+#             */
/*   Updated: 2026/08/27 13:36:40 by mweghofe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LISTENINGSOCKET_HPP
# define LISTENINGSOCKET_HPP

// -------------------------------------------------------------------------- //

# define BACKLOG 64		// how many unaccepted connections the kernel queues

#include <netinet/in.h>		// struct sockaddr_in

// -------------------------------------------------------------------------- //

class ListeningSocket
{
	public:
		// ----
		ListeningSocket(unsigned short);
		~ListeningSocket();
		// ----
		// ----
		int				getFD() const;
		int				getPort() const;
		int				acceptConnection(struct sockaddr_in&) const;
	private:
		// ----
		int				_fd;	// the fd for the server's own listening socket
		unsigned short	_port;	// the server port for listening socket
		struct sockaddr_in _ipAddr;
		int			  	_createNewSocket();
		void 			_configureFD();
		void			_bindAddrToFD();
		// ----
		// ----
		ListeningSocket();
		ListeningSocket(const ListeningSocket&);
		ListeningSocket operator=(const ListeningSocket&);
};

// -------------------------------------------------------------------------- //

#endif
