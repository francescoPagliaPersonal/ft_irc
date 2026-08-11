/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ListeningSocket.hpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mweghofe <mweghofe@student.42vienna.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/09 16:19:04 by mweghofe          #+#    #+#             */
/*   Updated: 2026/08/11 17:35:59 by mweghofe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LISTENINGSOCKET_HPP
# define LISTENINGSOCKET_HPP

// -------------------------------------------------------------------------- //

# define BACKLOG 64		// how many unaccepted connections the kernel queues

// -------------------------------------------------------------------------- //

class ListeningSocket
{
	public:
		// ----
		ListeningSocket(unsigned short);
		~ListeningSocket();
		// ----
		// ----
		int getFD() const;
		int acceptConnection(struct sockaddr_in&);
	private:
		// ----
		int		_fd;
		// ----
		// ----
		ListeningSocket();
		ListeningSocket(const ListeningSocket&);
		ListeningSocket operator=(const ListeningSocket&);
};

// -------------------------------------------------------------------------- //

#endif
