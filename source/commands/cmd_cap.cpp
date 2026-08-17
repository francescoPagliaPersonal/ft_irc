/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_cap.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fpaglia <fpaglia@student.42vienna.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/12 13:54:12 by fpaglia           #+#    #+#             */
/*   Updated: 2026/08/17 14:53:01 by fpaglia          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


# include "Command.hpp"
# include "IServerCtrl.hpp"
# include "Client.hpp"

int cmd_cap(IServerCtrl & srv, const Message & msg)
{
	// TODO need more content? currently is empty stub to advance handshake
	
	Client *client = msg.sender;
	srv.sendMessage(*client, ":CoolServ CAP * LS :\r\n");
	
	return (rfc::OK);
}
