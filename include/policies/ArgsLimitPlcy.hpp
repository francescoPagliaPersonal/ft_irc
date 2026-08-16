/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ArgsLimitPlcy.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mweghofe <mweghofe@student.42vienna.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/12 13:27:46 by fpaglia           #+#    #+#             */
/*   Updated: 2026/08/16 11:27:04 by mweghofe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ARGSLIMITPLCY_HPP
# define ARGSLIMITPLCY_HPP

#include "IPolicy.hpp"

class ArgsLimitPlcy : public IPolicy
{
	public:
	ArgsLimitPlcy(int minCount, int maxCount) 
		: _min(minCount), _max(maxCount) {};
	~ArgsLimitPlcy() {};

	int check(const Message & msg, IServerCtrl& srv) const
	{
		Client *client = msg.sender;
		if (client == NULL)
			return 3; //ERR_HANGHUP
		(void) srv;
		int args = argCount(msg);
		if (args < _min)
			return 1; // ERR_NEEDMOREPARAM
		else if (args > _max)
			return 2; // ERR_TOOMANYPARAM
		return 0;
	};
	private:
	int	_min;
	int _max;
	ArgsLimitPlcy();

};

#endif // ARGSLIMITPLCY_HPP
