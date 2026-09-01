/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_mode.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mweghofe <mweghofe@student.42vienna.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/27 16:57:46 by fpaglia           #+#    #+#             */
/*   Updated: 2026/09/01 14:22:24 by mweghofe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Channel.hpp"
#include "Client.hpp"
#include "Command.hpp"
#include "IServerCtrl.hpp"
#include "Message.hpp"
#include "irc.hpp"
#include "Response.hpp"

// -------------------------------------------------------------------------- //

namespace
{
	// main operation
	
	rfc handleUserMode(Command::Data& data);
	rfc handleChannelMode(Command::Data& data);
	void processModeRequests(Command::Data& data);
}

namespace helper
{
	// main operation

	bitMask handleModeChange(Command::Data& data, bool switcher, char c, std::size_t* iParams);

	// output helper

	void sendChannelModes(Command::Data& data);
	void sendUnknownMode(Command::Data&data, char c);
	void buildReply(Command::Data& data, std::string& rpl, bitMask valueTokens, bitMask old);
}

// -------------------------------------------------------------------------- //

// Dispatch MODE to a channel, an irssi user-mode stub, or 501.
rfc cmd_mode(IServerCtrl& srv, const Message& msg)
{
	Command::Data data(srv, msg, msg.sender);
	// FIXME this assumes, trailing is copied into param
	// we CANNOT get here w/o either params or trailing used! correct?
	// the param[0] existed elsewhere too, don't remember what the solution was
	if (msg.params[0][0] == '#' || msg.params[0][0] == '&')
		return (handleChannelMode(data));
	else if (msg.params[0] == data.client->getNick())
		return (handleUserMode(data));
	else
		return (irc::USERSDONTMATCH);
}

// -------------------------------------------------------------------------- //

namespace
{

// Answer irssi's MODE NICK +i with a dummy 221 so NICKNAMEINUSE logon can finish.
rfc handleUserMode(Command::Data& data)
{
	data.srv.sendMessage(data.client,
		Response::buildNumeric(data.msg, irc::UMODEIS, "", "not supported"));
	return (irc::OK);
}

// Query the channel's modes, or apply MODE if the sender is a member and op.
rfc handleChannelMode(Command::Data& data)
{
	
	data.channel = data.srv.getChannelByTitle(data.msg.params[0]);
	
	// 1) channel validation
	if (data.channel == NULL)
		return (irc::NOSUCHCHANNEL);
	// 2) mode query
	if (data.msg.params.size() == 1)
	{
		helper::sendChannelModes(data);
		return (irc::OK);
	}
	// 3) validate member
	if (!data.channel->isMember(data.client))
		return (irc::NOTONCHANNEL);
	// 4) OP validation
	if (!data.channel->isChanOp(data.client))
		return (irc::CHANOPRIVSNEEDED);
	// 5) check all params
	processModeRequests(data);
	return (irc::OK);
}

// Traverse all mode letters in msg.param, apply each change, broadcast the MODE line.
void processModeRequests(Command::Data& data)
{
	bool switcher = false;
	std::string reply(" ");
	bitMask modes, valueTokens = 0;
	modes = data.channel->getModes();
	// move through all params
	for (std::size_t i = 1; i < data.msg.params.size(); i++)
	{
		const std::string& param = data.msg.params[i];
		// validate starting point (just ignore string w/o +/-)
		if (param.empty() || (param[0] != '+' && param[0] != '-'))
			continue;
		// consume one char at a time
		for (std::size_t n = 0; n < param.size(); n++)
		{
			// get sign within the mode string
			if ((param[n] == '+' || param[n] == '-'))
			{
				switcher = (param[n] == '+');
				continue ;
			}
			// look for registered modes modes
			if (std::string(MODES).find(param[n]) == std::string::npos)
				helper::sendUnknownMode(data, param[n]);
			else
				valueTokens |= helper::handleModeChange(data, switcher, param[n], &i);
		}
	}
	helper::buildReply(data, reply, valueTokens, modes);
	if (reply.size() > 1) // contains a ' ' per default
		data.srv.broadcast(data.channel,
			Response::buildRegular(data.msg,
								   data.channel->getTitle() + reply, ""));
}

} // end of namespace
