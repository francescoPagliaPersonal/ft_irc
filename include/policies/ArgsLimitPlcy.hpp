/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ArgsLimitPlcy.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mweghofe <mweghofe@student.42vienna.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/12 13:27:46 by fpaglia           #+#    #+#             */
/*   Updated: 2026/08/16 14:48:58 by mweghofe         ###   ########.fr       */
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

	int check(const Message & msg, IServerCtrl& srv) const;
	private:
	int	_min;
	int _max;
	ArgsLimitPlcy();

};

#endif // ARGSLIMITPLCY_HPP
