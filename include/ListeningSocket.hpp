/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ListeningSocket.hpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mweghofe <mweghofe@student.42vienna.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/09 16:19:04 by mweghofe          #+#    #+#             */
/*   Updated: 2026/08/09 16:24:58 by mweghofe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LISTENINGSOCKET_HPP
# define LISTENINGSOCKET_HPP

// -------------------------------------------------------------------------- //

// -------------------------------------------------------------------------- //

class ListeningSocket
{
	public:
		// ----
		ListeningSocket(int);
		~ListeningSocket();
		// ----
		// ----
	private:
		// ----
		int		fd_;
		// ----
		ListeningSocket();
		ListeningSocket(const ListeningSocket&);
		ListeningSocket operator=(const ListeningSocket&);
};

// -------------------------------------------------------------------------- //

#endif