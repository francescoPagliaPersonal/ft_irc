/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   IPolicy.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fpaglia <fpaglia@student.42vienna.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/12 11:30:15 by fpaglia           #+#    #+#             */
/*   Updated: 2026/08/12 11:30:17 by fpaglia          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef IPOLICY_HPP
# define IPOLICY_HPP

#include "Command.hpp"

class Server;

class IPolicy
{
	public:
	IPolicy() {};
	virtual ~IPolicy() {};

	virtual int		check(const Message &, Server&) const = 0;

	private:
	IPolicy(const IPolicy & other);
	IPolicy & operator=(const IPolicy & other);

};

#endif // !IPOLICY_HPP
