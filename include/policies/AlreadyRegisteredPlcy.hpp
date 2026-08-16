/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   AlreadyRegisteredPlcy.hpp                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mweghofe <mweghofe@student.42vienna.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/12 13:36:47 by fpaglia           #+#    #+#             */
/*   Updated: 2026/08/16 14:47:57 by mweghofe         ###   ########.fr       */
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

	int check(const Message & msg, IServerCtrl& srv) const;

	private:
	bool	_status;

};

#endif