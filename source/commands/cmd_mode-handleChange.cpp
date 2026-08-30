/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_mode-handleChange.cpp                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mweghofe <mweghofe@student.42vienna.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/29 23:12:59 by mweghofe          #+#    #+#             */
/*   Updated: 2026/08/30 12:31:06 by mweghofe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "irc.hpp"
#include "Channel.hpp"
#include "Command.hpp"
#include "IServerCtrl.hpp"
#include "Message.hpp"
#include "Response.hpp"

#include <cstdlib>

/*

i: Set/remove Invite-only channel.
t: Set/remove restrictions on the TOPIC command.
k: Set/remove the channel key (password).			+   needs ARG
o: Give/take channel operator privilege.			+/- need ARG
l: Set/remove the user limit for the channel.		+   needs ARG

	some observations from testing with !! INSPIRCD !!:
	- key must be removed before a new one can be set again
	- another +k just gets ignored
	- removal of key prints old key
	- operators don't show in MODE #channel
	- limit can be replaced
	- output is +iklt key :limit
	- multi removal is possible (non existing get ignored)
	- multi setting is possible, failure occurs individually
	- when sending +kl at the same time, the order doesn't matter
		the values are taken as <key> <limit>, if limit is not a number -> 0

FURTHER NOTES

- there is sth about Type C and earlier types
  eg. RFC 2812 Type B clients will use `-k key` instead of `-k`
  => i don't think we want to care

*/

namespace
{
	// Steal the next MSG param for a mode that needs an argument; send 461 if none remain.
	bool canConsumeNextParam(Command::Data& data, std::size_t* iParams)
	{
		if (++(*iParams) >= data.msg.params.size())
		{
			data.srv.sendMessage(
				data.client,
				Response::buildNumeric(
					data.msg,
					// TODO ?? IRCv3 offers 696 + mode specific message
					irc::NEEDMOREPARAMS,
					data.msg.command + " " + data.channel->getTitle())
			);
			return (false);
		}
		return (true);
	}
} // end of namespace

namespace helper
{

// Apply mode letter C with SWITCHER to the channel, consuming an argument when needed.
bitMask handleModeChange(Command::Data& data, bool switcher, char c, std::size_t* iParams)
{
	bitMask valueToken = 0;
	// ---------- LAZY MODE ----------
	// every operation is run, duplicates are NOT ignored
	// except ops with args, which must guard their value themselves
	switch (c)
	{
		case 'i':
			data.channel->setInvite(switcher);
			break ;

		case 't':
			data.channel->setTopicFlag(switcher);
			break ;

		case 'k':
			if (switcher == false)
			{
				data.channel->removePassword();
				break ;
			}
			if (!canConsumeNextParam(data, iParams))
				break ;
			if (data.channel->getModes() & CH_PASSWORD)
				break ;
			data.channel->setPassword(data.msg.params[*iParams]);
			valueToken = CH_PASSWORD;
			break ;

		case 'o':
		{
			if (!canConsumeNextParam(data, iParams))
				break ;
			Client* op = data.srv.findClientByNick(data.msg.params[*iParams]);
			if (!op)
			{
				data.srv.sendMessage(data.client,
					Response::buildNumeric(data.msg, irc::NOSUCHNICK,
						data.msg.params[*iParams])
				);
				break ;
			}
			// TODO ?? inspircd doesnt print this at all, silent fail
			if (!data.channel->isMember(op))
			{
				data.srv.sendMessage(data.client,
					Response::buildNumeric(data.msg, irc::USERNOTINCHANNEL,
						data.msg.params[*iParams] + " " + data.channel->getTitle())
				);
				break ;
			}
			bool changed = data.channel->setOperator(switcher, op);
			if (changed)
				valueToken = US_OPERATOR; // this is not clean, but uncontested in buildReply
			if (changed && switcher)
				data.modeChOPadd.push_back(op->getNick());
			else if (changed && !switcher)
				data.modeChOPrem.push_back(op->getNick());
			break ;
		}

		case 'l':
			if (switcher == false)
			{
				data.channel->removeLimit();
				break ;
			}
			if (!canConsumeNextParam(data, iParams))
				break ;
			data.channel->setLimit(std::strtol(data.msg.params[*iParams].c_str(), NULL, 10));
			valueToken = CH_LIMIT;
			break ;

	}
	return (valueToken);
}

} // end of namespace HELPER
