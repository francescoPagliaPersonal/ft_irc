/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_invite.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fpaglia <fpaglia@student.42vienna.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/27 16:10:11 by fpaglia           #+#    #+#             */
/*   Updated: 2026/08/27 16:10:43 by fpaglia          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <vector>

#include "Channel.hpp"
#include "Command.hpp"
#include "IServerCtrl.hpp"
#include "Client.hpp"
#include "Message.hpp"
#include "irc.hpp"
#include "Response.hpp"

rfc cmd_invite(IServerCtrl& srv, const Message& msg)
{
	Client* sender = msg.sender;
	Channel * channel = srv.getChannelByTitle(msg.params[1]);

	if (channel == NULL)
		return irc::NOSUCHCHANNEL;
	Client * invitee = srv.findClientByNick(msg.params[0]);
	if (invitee == NULL)
		return irc::NOSUCHNICK;
	if (channel->isMember(invitee))
		return irc::USERONCHANNEL;
	if (!sender->isChannelMember(channel))
		return irc::NOTONCHANNEL;
	if ((channel->getModes() & CH_INVITE) && !channel->isChanOp(sender))
		return irc::CHANOPRIVSNEEDED;

	channel->invite(invitee);
	//:dan-!d@localhost INVITE Wiz #test 
	srv.sendMessage(invitee, Response::buildRegular(msg, invitee->getNick() + " " + channel->getTitle()));
	srv.sendMessage(sender, Response::buildNumeric(msg, irc::INVITING, invitee->getNick() + " " + channel->getTitle()));
	return irc::OK;
}
