/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Epoll.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mweghofe <mweghofe@student.42vienna.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/09 16:56:53 by mweghofe          #+#    #+#             */
/*   Updated: 2026/08/09 21:59:15 by mweghofe         ###   ########.fr       */
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
		void add(int, eventflags) const;
		void mod();
		void del();
		int wait(struct epoll_event*, int, int);
	private:
		// ----
		int		fd_;
		// ----
		// ----
		Epoll(const Epoll&);
		Epoll operator=(const Epoll&);
};

// -------------------------------------------------------------------------- //

#endif
