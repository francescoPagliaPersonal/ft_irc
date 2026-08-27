/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client-getMessages.cpp                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mweghofe <mweghofe@student.42vienna.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/12 09:16:39 by fpaglia           #+#    #+#             */
/*   Updated: 2026/08/26 15:41:43 by mweghofe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_irc.hpp"
#include "Client.hpp"

// Split the input buffer into raw message strings at CRLF and return them.
std::vector<std::string>		Client::getRawStrings()
{
	std::vector<std::string>	msgs;
	std::string::size_type		pos = 0;

	while (pos != std::string::npos)
	{
		pos = _bufIN.find(CRLF, 0);
		if (pos == std::string::npos)
		{
			if (_bufIN.size() > MSG_MAX_LENGTH)
				msgs.push_back(_bufIN);
			break;
		}
		msgs.push_back(_bufIN.substr(0, pos));
		// _bufIN = _bufIN.substr(pos + 2);
		_bufIN.erase(0, pos + 2 );
	}
	return msgs;
}
