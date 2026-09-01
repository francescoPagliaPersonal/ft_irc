/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_mode-response.cpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mweghofe <mweghofe@student.42vienna.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/29 23:04:12 by mweghofe          #+#    #+#             */
/*   Updated: 2026/09/01 18:05:03 by mweghofe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "irc.hpp"
#include "Command.hpp"
#include "IServerCtrl.hpp"
#include "Message.hpp"
#include "Response.hpp"

#include <sstream>

// -------------------------------------------------------------------------- //

namespace
{

// Drop nicks that appear in both ADDOP and REMOP so a cancelled +/-o is not announced.
void cancelOperatorNoop(std::deque<std::string>& addOP,
							 std::deque<std::string>& remOP)
{
	if (addOP.empty() || remOP.empty())
		return ;
	std::deque<std::string>::iterator itAdd, itRem;
	itAdd = addOP.begin();
	while (itAdd != addOP.end())
	{
		bool found = false;
		for (itRem = remOP.begin(); itRem != remOP.end(); itRem++)
		{
			if (*itAdd == *itRem)
			{
				remOP.erase(itRem);
				itAdd = addOP.erase(itAdd);
				found = true;
				break ;
			}
		}
		if (!found)
			itAdd++;
	}
}

} // end of namespace

// -------------------------------------------------------------------------- //

namespace helper
{

// Send 324 RPL_CHANNELMODEIS with the channel's current modes to the sender.
void sendChannelModes(Command::Data& data)
{
	std::string reply(data.channel->getTitle() + " +");
	std::string pw;
	std::stringstream limit;
	bitMask modes = data.channel->getModes();
	if (modes & CH_INVITE)
		reply.append("i");
	if (modes & CH_TOPIC)
		reply.append("t");
	if (modes & CH_PASSWORD)
	{
		reply.append("k");
		if (data.channel->isMember(data.client))
			pw.append(" " + data.channel->getPassword());
	}
	if (modes & CH_LIMIT)
	{
		reply.append("l");
		limit << data.channel->getLimit();
	}
	data.srv.sendMessage(data.client,
		Response::buildNumeric(data.msg, irc::CHANNELMODEIS,
			reply + pw, limit.str())
	);
	// TODO decide if we want to store timestamp or create; MODE can return it
}

// Send 472 ERR_UNKNOWNMODE for the unknown letter C.
void sendUnknownMode(Command::Data& data, char c)
{
	data.srv.sendMessage(
		data.msg.sender,
		Response::buildNumeric(
			data.msg,
			irc::UNKNOWNMODE,
			std::string(1, c),
			"is unknown mode char to me for " + data.channel->getTitle()
		)
	);
}

// Append the net MODE string to RPL from OLD flags, VALUETOKENS, and queued operator nicks.
void buildReply(Command::Data& data, std::string& rpl, bool specialConsideration, bitMask old)
{
	bitMask changed, added, removed, now;
	static irc::uint lastLimit;
	now = data.channel->getModes();
	// ---- abort if no change -------------------------------------------------
	if (old == now && !specialConsideration)
		return ;
	// ---- prepare ------------------------------------------------------------
	std::string values, plus, minus, addOP, remOP;
	changed = old ^ now;		// shows the bits that flipped
	added = changed & now;		// what went from 0 to 1
	removed = changed & old; 	// what wend from 1 to 0
	// ---- collect the ADDED ones ---------------------------------------------
	if (added & CH_INVITE)		plus  += 'i';
	if (added & CH_TOPIC)		plus  += 't';
	if (added & CH_PASSWORD)
	{
		plus  += 'k';
		values += ' ' + data.channel->getPassword();
	}
	if (added & CH_LIMIT)
	{
		plus  += 'l';
		lastLimit = data.channel->getLimit();
		std::ostringstream ss;
		ss << lastLimit;
		values += ' ' + ss.str();
	}
	// ---- collect the REMOVED ones -------------------------------------------
	if (removed & CH_INVITE)	minus += 'i';
	if (removed & CH_TOPIC)		minus += 't';
	if (removed & CH_PASSWORD)	minus += 'k';
	if (removed & CH_LIMIT)		minus += 'l';
	// ---- get updated values -------------------------------------------------
	if (specialConsideration )
	{
		if ((now & CH_LIMIT) && !(added & CH_LIMIT))
		{
			plus += 'l';
			std::ostringstream ss;
			ss << data.channel->getLimit();
			values += ' ' + ss.str();
		}
		cancelOperatorNoop(data.modeChOPadd, data.modeChOPrem);
	}
	// ---- build the reply ----------------------------------------------------
	if (!minus.empty() || !data.modeChOPrem.empty())
		rpl += '-' + minus;
	while (!data.modeChOPrem.empty())
	{
		rpl += "o";
		remOP += ' ' + data.modeChOPrem.front();
		data.modeChOPrem.pop_front();
	}
	if (!plus.empty() || !data.modeChOPadd.empty())
	{
		rpl += '+' + plus;
	}
	while (!data.modeChOPadd.empty())
	{
		rpl += "o";
		addOP += ' ' + data.modeChOPadd.front();
		data.modeChOPadd.pop_front();
	}
	rpl += remOP + values + addOP;
}

} // end of namespace HELPER
