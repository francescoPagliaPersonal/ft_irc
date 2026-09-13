/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ListeningSocket.hpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mweghofe <mweghofe@student.42vienna.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/09 16:19:04 by mweghofe          #+#    #+#             */
/*   Updated: 2026/09/13 12:59:09 by mweghofe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LISTENINGSOCKET_HPP
# define LISTENINGSOCKET_HPP

// -------------------------------------------------------------------------- //

#include <string>

#include <netinet/in.h>		// struct sockaddr_in

// -------------------------------------------------------------------------- //

# define BACKLOG 64	// how many unaccepted connections the kernel queues
# define ALIVE_TIME_IDLE 50 // idle time after last data packet
# define ALIVE_TIME_INTERVAL 4 // time between keepalive probes
# define ALIVE_PROBES 3 // number of probes to send

// -------------------------------------------------------------------------- //

class ListeningSocket
{
	public:
		// ----
		ListeningSocket(unsigned short);
		~ListeningSocket();
		// ----
		enum acceptret
		{
			ACCEPT_SETUP_FAIL = -2,
			ACCEPT_NONE = -1
		};
		// ----
		int				getFD() const;
		int				getPort() const;
		std::string		getHostName() const;
		int				acceptConnection(struct sockaddr_in&) const;

	private:
		// ----
		int				_fd;	// the fd for the server's own listening socket
		unsigned short	_port;	// the server port for listening socket
		std::string		_hostName;
		struct sockaddr_in _ipAddr;
		int			  	_createNewSocket();
		void 			_configureFD();
		void			_bindAddrToFD();
		void 			_setHostName();
		void			_errorOnAcceptConnection(struct sockaddr_in&) const;
		bool			_enableKeepAlive(int) const;

		// ----
		// ----
		ListeningSocket();
		ListeningSocket(const ListeningSocket&);
		ListeningSocket operator=(const ListeningSocket&);
};

// -------------------------------------------------------------------------- //

#endif
