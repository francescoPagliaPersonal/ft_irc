/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Message.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fpaglia <fpaglia@student.42vienna.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/09 17:38:21 by mweghofe          #+#    #+#             */
/*   Updated: 2026/08/24 18:37:32 by fpaglia          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MESSAGE_HPP
# define MESSAGE_HPP

// -------------------------------------------------------------------------- //
# include "Client.hpp"

# include <string>
# include <vector>

// -------------------------------------------------------------------------- //

enum e_msgflags
{
	MSG_HAS_PREFIX = 1 << 0,
	MSG_HAS_COMMAND = 1 << 1,
	MSG_HAS_PARAMS = 1 << 2,
	MSG_HAS_TRAILING = 1 << 3
};

// -------------------------------------------------------------------------- //

// Structure to separate data according to IRC's RFC protocol.
struct Message
{
	unsigned char				flags;						
	std::string					prefix;
	std::string					command;
	std::vector<std::string>	params;
	std::string					trailing;
	
	Client *					sender;
};

// -------------------------------------------------------------------------- //

Message	string2Message(std::string str, Client *client);
int		argCount(const Message &);

#endif
