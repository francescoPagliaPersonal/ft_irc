/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_pong.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mweghofe <mweghofe@student.42vienna.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/10 14:07:51 by mweghofe          #+#    #+#             */
/*   Updated: 2026/09/15 15:37:16 by mweghofe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_irc.hpp"
#include "Command.hpp"
#include "irc.hpp"
#include "Client.hpp"
#include "IServerCtrl.hpp"
#include "Response.hpp"

/// Prevents client disconnect by resetting ping counter when valid PONG is received.
rfc cmd_pong(IServerCtrl& srv, const Message& msg)
{
	(void) srv;
	// IMPORTANT: an INVALID PING does not reset the counter!
	//			  we could change this, by resetting here on top, no matter what
	//			  but this design is strict and requires a correct attribute
	Client* client = msg.sender;


	if (msg.params[0] != client->getNick())
		return (irc::NOORIGIN);
	// internal timer is updated by Server::_executeCommands
	client->resetPingCount();
	return (irc::OK);
}
