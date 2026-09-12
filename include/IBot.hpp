/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   IBot.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mweghofe <mweghofe@student.42vienna.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/07 17:14:08 by mweghofe          #+#    #+#             */
/*   Updated: 2026/09/12 10:15:44 by mweghofe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef IBOT_HPP
# define IBOT_HPP

# include <string>

class IBot
{
	public:
		virtual void sendMessage(const std::string&) = 0;
		virtual void toggleMirror() = 0;
};

#endif
