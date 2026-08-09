/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Message.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mweghofe <mweghofe@student.42vienna.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/09 17:38:21 by mweghofe          #+#    #+#             */
/*   Updated: 2026/08/09 17:44:54 by mweghofe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MESSAGE_HPP
# define MESSAGE_HPP

// -------------------------------------------------------------------------- //

# include <string>
# include <vector>

// -------------------------------------------------------------------------- //

struct Message
{
	std::string					prefix_;
	std::string					command_;
	std::vector<std::string>	params_;
	std::string					trailing_;
};

// -------------------------------------------------------------------------- //

Message parseMessage(const std::string&);

#endif
