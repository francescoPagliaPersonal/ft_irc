/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   AlredyRegisteredPlcy.hpp                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mweghofe <mweghofe@student.42vienna.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/12 13:36:47 by fpaglia           #+#    #+#             */
/*   Updated: 2026/08/14 03:05:18 by mweghofe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ALREADYREGISTEREDPLCY_HPP
# define ALREADYREGISTEREDPLCY_HPP

#include "Client.hpp"
#include "IPolicy.hpp"


class AlredyRegisteredPlcy : public IPolicy
{
	public:
	AlredyRegisteredPlcy(bool status): _status(status) {};
	~AlredyRegisteredPlcy() {};

	int check(const Message & msg, Server& srv) const
	{
		(void) srv;
		Client *client = msg.sender;
		if (client == NULL)
			return 3; //ERR_HANGHUP
		if (_status && client->getRegistrationFlags() != REG_DONE)
			return 1; // ERR_ALREADYREGISTERED
		return 0;
	};

	private:
	bool	_status;

};

#endif