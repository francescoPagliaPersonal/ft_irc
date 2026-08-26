/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CommandRegistry-handleProtocolErrors.cpp           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mweghofe <mweghofe@student.42vienna.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/17 16:52:31 by mweghofe          #+#    #+#             */
/*   Updated: 2026/08/26 17:51:52 by mweghofe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Client.hpp"
#include "CommandRegistry.hpp"
#include "IServerCtrl.hpp"
#include "ft_irc.hpp"
#include "irc.hpp"
#include <sstream>
#include "Response.hpp"

// Send the proper protocol reply for NUMERIC to the message's sender;
// returns false if the client should be disconnected.
bool CommandRegistry::handleProtocolErrors(IServerCtrl& srv, rfc numeric,
	const Message& msg)
{
	if (!msg.sender)
		return (false);
	if (numeric != irc::OK)
		srv.sendMessage(*msg.sender, Response::handleNumeric(msg, numeric));
	
	return (true);
}
