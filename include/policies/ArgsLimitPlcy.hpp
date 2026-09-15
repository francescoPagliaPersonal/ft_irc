/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ArgsLimitPlcy.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fpaglia <fpaglia@student.42vienna.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/12 13:27:46 by fpaglia           #+#    #+#             */
/*   Updated: 2026/09/15 17:30:53 by fpaglia          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ARGSLIMITPLCY_HPP
# define ARGSLIMITPLCY_HPP

# include "IPolicy.hpp"

class ArgsLimitPlcy : public IPolicy
{
	public:
	ArgsLimitPlcy(int minCount, int maxCount, bool ignoreTrail) 
		: _min(minCount), _max(maxCount), _ignoreTrail(ignoreTrail) {};
	~ArgsLimitPlcy() {};

	rfc check(const Message & msg) const;
	private:
	int	_min;
	int _max;
	bool _ignoreTrail;
	ArgsLimitPlcy();

};

#endif // ARGSLIMITPLCY_HPP
