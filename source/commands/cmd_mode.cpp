/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_mode.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mweghofe <mweghofe@student.42vienna.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/27 16:57:46 by fpaglia           #+#    #+#             */
/*   Updated: 2026/08/29 12:46:01 by mweghofe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Channel.hpp"
#include "Client.hpp"
#include "Command.hpp"
#include "IServerCtrl.hpp"
#include "Message.hpp"
#include "irc.hpp"
#include "Response.hpp"

#include <sstream>
#include <cstdlib>

// -------------------------------------------------------------------------- //

namespace
{
	// compatibility fix

	rfc handleIrssiLogon(Command::Data& data);

	// main operation

	rfc handleChannelMode(Command::Data& data);
	void processModeRequests(Command::Data& data);
	std::string handleModeChange(Command::Data& data, bool switcher, char c, std::size_t* iParams);
	
	// output helper

	void printChannelModes(Command::Data& data);
	void sendUnknownMode(Command::Data&data, char c);
	void buildReply(Command::Data& data, std::string& reply, std::string& replyValues);
}

rfc cmd_mode(IServerCtrl& srv, const Message& msg)
{
	Command::Data data(srv, msg, msg.sender);
	// TODO this assumes, trailing is copied into param
	// we CANNOT get here w/o either params or trailing used! correct?
	if (msg.params[0][0] == '#' || msg.params[0][0] == '&')
		return (handleChannelMode(data));
	// TODO fine like that? if we have this as a DUMMY,
	//		 irssi will register the correct nick on NICKNAMEINUSE
	else if (msg.params[0] == data.client->getNick())
		return (handleIrssiLogon(data));
	else
		return (irc::UMODEUNKNOWNFLAG);
}

// -------------------------------------------------------------------------- //

namespace
{

rfc handleIrssiLogon(Command::Data& data)
{
	if (data.msg.params.size() == 2 && data.msg.params[1] == "+i")
	{
		// either of those stubs fixes the irssi NICKNAMEINUSE interface update issue
		// error message does NOT
		data.srv.sendMessage(data.client,
			Response::buildNumeric(data.msg, irc::UMODEIS, "", "not supported"));
			// Response::buildNumeric(msg, irc::UMODEIS, "+i"));
			// Response::buildRegular(msg, client->getNick(), msg.params[1]));
		return (irc::OK);
	}
	else
		return (irc::UMODEUNKNOWNFLAG);
}

rfc handleChannelMode(Command::Data& data)
{
	
	data.channel = data.srv.getChannelByTitle(data.msg.params[0]);
	
	// 1) channel validation
	if (data.channel == NULL)
		return (irc::NOSUCHCHANNEL);
	// 2) mode query
	if (data.msg.params.size() == 1)
	{
		printChannelModes(data);
		return (irc::OK);
	}
	// 3) OP validation
	if (!data.channel->isChanOp(data.client))
		return (irc::CHANOPRIVSNEEDED);
	// 4) check all params
	processModeRequests(data);
	return (irc::OK);
}

void processModeRequests(Command::Data& data)
{
	bool switcher = false;
	std::string reply, replyValues;
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
				sendUnknownMode(data, param[n]);
			else
				replyValues = handleModeChange(data, switcher, param[n], &i);
		}
	buildReply(data, reply, replyValues);
	}
	data.srv.broadcast(data.channel, reply);
}

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

		*/

std::string handleModeChange(Command::Data& data, bool switcher, char c, std::size_t* iParams)
{
	// LAZY MODE -- every operation is run, duplicates are NOT ignored
	//				except ops with args, which must guard their value themselves
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
			if (++(*iParams) >= data.msg.params.size())
			{
				// TODO send error message;
				break ;
			}
			data.channel->setPassword(data.msg.params[*iParams]);
			break ;
		case 'o':
		{
			if (++(*iParams) >= data.msg.params.size())
			{
				// TODO send error message;
				break ;
			}
			Client* op = data.srv.findClientByNick(data.msg.params[*iParams]);
			if (!op)
			{
				// TODO send error message;
				break ;
			}
			if (switcher == true)
				data.channel->addOperator(op);
			else
				data.channel->removeOperator(op);
			break ;
		}
		case 'l':
			if (switcher == false)
			{
				data.channel->removeLimit();
				break ;
			}
			if (++(*iParams) >= data.msg.params.size())
			{
				// TODO send error message;
				break ;
			}
			data.channel->setLimit(std::strtol(data.msg.params[*iParams].c_str(), NULL, 10));
			break ;
	}
	// TODO need to watch for key, limit, operator - need ARGS!
	// TODO all modes with switcher & message & ignore duplicates
	return ("");
}

void printChannelModes(Command::Data& data)
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
		pw.append(" " + data.channel->getPassword());
	}
	if (modes & CH_LIMIT)
	{
		reply.append("l");
		limit << data.channel->getLimit();
	}
	data.srv.sendMessage(data.client,
		Response::buildRegular(data.msg, reply + pw, limit.str())
	);
	// TODO decide if we want to store timestamp or create; MODE can return it
}

void sendUnknownMode(Command::Data&data, char c)
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

void buildReply(Command::Data& data, std::string& reply, std::string& replyValues)
{
	(void) data;
	// TODO compare changes in mode
	// TODO build mode change string
	reply.append(replyValues);
	replyValues.clear();
}

} // end of namespace
