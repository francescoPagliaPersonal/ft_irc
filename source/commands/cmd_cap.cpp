/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_cap.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mweghofe <mweghofe@student.42vienna.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/12 13:54:12 by fpaglia           #+#    #+#             */
/*   Updated: 2026/08/14 10:25:02 by mweghofe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


# include "Command.hpp"
# include "Server.hpp"
# include "Client.hpp"

int cmd_cap(Server & srv, const Message & msg)
{
	// TODO: this is just a quick proof of concept more complex UserName evaluation 
	// should be carried out (against the whole server).
	
	
	Client *client = msg.sender;
	if (client == NULL)
		return 1; // ERR_HANGUP??
	client->putReply2Buff(srv, ":CoolServ  CAP * LS :\r\n");
	
	return (0);
}
