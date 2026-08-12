/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Epoll.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mweghofe <mweghofe@student.42vienna.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/09 16:56:53 by mweghofe          #+#    #+#             */
/*   Updated: 2026/08/12 13:25:45 by mweghofe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EPOLL_HPP
# define EPOLL_HPP

// -------------------------------------------------------------------------- //

# include <sys/epoll.h>

// -------------------------------------------------------------------------- //

typedef uint32_t eventflags;

class Epoll
{
	public:
		// ----
		Epoll();
		~Epoll();
		// ----
		// ----
		void add(int, eventflags, void*) const;
		void mod(int, eventflags, void*) const;
		void del(int) const;
		int wait(struct epoll_event*, int, int);
	private:
		// ----
		int		_fd;
		// ----
		// ----
		Epoll(const Epoll&);
		Epoll operator=(const Epoll&);
};

// -------------------------------------------------------------------------- //

#endif
