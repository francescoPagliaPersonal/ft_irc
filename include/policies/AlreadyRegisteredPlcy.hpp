/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   AlreadyRegisteredPlcy.hpp                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mweghofe <mweghofe@student.42vienna.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/12 13:36:47 by fpaglia           #+#    #+#             */
/*   Updated: 2026/08/17 12:10:20 by mweghofe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ALREADYREGISTEREDPLCY_HPP
# define ALREADYREGISTEREDPLCY_HPP

#include "Client.hpp"
#include "IPolicy.hpp"


class AlreadyRegisteredPlcy : public IPolicy
{
	public:
	AlreadyRegisteredPlcy(bool status): _status(status) {};
	~AlreadyRegisteredPlcy() {};

	int check(const Message & msg, IServerCtrl& srv) const;

	private:
	bool	_status;

};

#endif
