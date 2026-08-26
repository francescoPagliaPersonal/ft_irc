/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CommandRegistry-handleProtocolErrors.cpp           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fpaglia <fpaglia@student.42vienna.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/17 16:52:31 by mweghofe          #+#    #+#             */
/*   Updated: 2026/08/26 14:14:58 by fpaglia          ###   ########.fr       */
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
		srv.sendMessage(*msg.sender, Response::noOpt(msg, numeric));
	
	return (true);
}
