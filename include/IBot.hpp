/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   IBot.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mweghofe <mweghofe@student.42vienna.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/07 17:14:08 by mweghofe          #+#    #+#             */
/*   Updated: 2026/09/12 16:00:52 by mweghofe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef IBOT_HPP
# define IBOT_HPP

# include <string>

# define BOT_NAME "Bot"
# define CRLF "\r\n" // duplicate of ft_irc.hpp
# define SPAM_COUNT 12
# define SPAM_MSG_42 "42 is the answer to the Ultimate Question of Life, the Universe, and Everything."
# define SPAM_MSG_CH "Have you tried turning it off and on again?"
# define HELP_L1 "Available commands are: !help, !quote, !mirror, !spam <target>"
# define HELP_L2 "  !help:   prints this message"
# define HELP_L3 "  !quote:  tell you a random quote"
# define HELP_L4 "  !mirror: on/off toggle to mirror every message received"
# define HELP_L5 "  !spam:   send many messages to current channel or <target> channel/user"

class IBot
{
	public:
		virtual void sendMessage(const std::string&) = 0;
		virtual void toggleMirror() = 0;
};

#endif
