/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_mode.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mweghofe <mweghofe@student.42vienna.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/27 16:57:46 by fpaglia           #+#    #+#             */
/*   Updated: 2026/09/02 07:15:08 by mweghofe         ###   ########.fr       */
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
	std::string buildReply(Command::Data& data, bool specialConsideration, bitMask old);
}

// -------------------------------------------------------------------------- //

// Dispatch MODE to a channel, an irssi user-mode stub, or 501.
rfc cmd_mode(IServerCtrl& srv, const Message& msg)
{
	Command::Data data(srv, msg, msg.sender);
	// param[0] is guaranteed to exist because of the policy
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

/*

	MODE INPUT STRING

	must always be: MODE <+/-commandLettersInAnyOrder> <arguments for commands>
	there is basically only a command string that can be followed by arguments
	irssi will transform any /mode input to match that!!
			/mode +i -i +i -i +i -i +i -i +k -i +i -i +l 42
	becomes MODE #world +i-i+i-i+i-i+i-i+ki-i+l -i 42\r\n

*/

// Traverse all mode letters in msg.param, apply each change, broadcast the MODE line.
void processModeRequests(Command::Data& data)
{
	bool switcher = false;						// true for add (+) modes
	bool specialConsideration = false;			// mode change w/o bitMask
	std::string reply;							// takes the success reply msg
	const std::string& modes(data.msg.params[1]);	// shorthand
	bitMask modesSet;							// flags on begin & special flag
	std::size_t argsPos = 1;					// start pos for args consumption

	modesSet = data.channel->getModes();
	// 1) validate starting point (just ignore string w/o +/-)
	if (modes[0] != '+' && modes[0] != '-')
	{
		helper::sendUnknownMode(data, modes[0]);
		return ;
	}
	// 2) consume one char at a time
	for (std::size_t n = 0; n < modes.size(); n++)
	{
		// get sign within the mode string
		if ((modes[n] == '+' || modes[n] == '-'))
		{
			switcher = (modes[n] == '+');
			continue ;
		}
		// look for registered modes modes
		if (std::string(MODES).find(modes[n]) == std::string::npos)
			helper::sendUnknownMode(data, modes[n]);
		else
			specialConsideration = helper::handleModeChange(data, switcher, modes[n], &argsPos);
	}
	// 3) build the reply string on a successful mode change
	reply = helper::buildReply(data, specialConsideration, modesSet);
	// 4) broadcast the reply
	if (reply.size())
		data.srv.broadcast(data.channel,
			Response::buildRegular(data.msg,
								   data.channel->getTitle() + reply, ""));
}

} // end of namespace
