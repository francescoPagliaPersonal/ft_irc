/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fake_server.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mweghofe <mweghofe@student.42vienna.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/24 10:48:00 by mweghofe          #+#    #+#             */
/*   Updated: 2026/08/31 10:33:00 by mweghofe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FAKE_SERVER_HPP
# define FAKE_SERVER_HPP

# include "Channel.hpp"
# include "Client.hpp"
# include "IServerCtrl.hpp"
# include "Response.hpp"
# include "ft_irc.hpp"
# include "irc.hpp"

# include <map>
# include <string>
# include <utility>
# include <vector>

class FakeServer : public IServerCtrl
{
public:
	std::string												password;
	std::map<std::string, Client*>							nicks;
	std::map<std::string, Channel*>							channels;
	mutable std::vector<std::pair<Client*, std::string> >	sent;
	mutable int												completeCalls;

	FakeServer()
		: completeCalls(0)
	{
		Response::init("CoolServ");
	}

	~FakeServer()
	{
		std::map<std::string, Channel*>::iterator	it;

		for (it = channels.begin(); it != channels.end(); ++it)
			delete it->second;
		channels.clear();
	}

	std::string	getPassword() const
	{
		return (password);
	}

	void	tryCompleteRegistration(Client *c) const
	{
		if (!c || c->getRegistrationFlags() != REG_DONE)
			return ;
		++completeCalls;
	}

	Client	*findClientByNick(const std::string &n) const
	{
		std::map<std::string, Client*>::const_iterator	it;

		it = nicks.find(n);
		if (it == nicks.end())
			return (0);
		return (it->second);
	}

	void	sendMessage(Client *c, const std::string &s) const
	{
		sent.push_back(std::make_pair(c, s));
	}

	void	broadcast(Channel *channel, Client *sender,
				const std::string &reply) const
	{
		std::map<Client*, bitMask>	members;

		if (!channel)
			return ;
		members = channel->getMembersMap();
		for (std::map<Client*, bitMask>::const_iterator it = members.begin();
			 it != members.end(); ++it)
		{
			if (it->first != sender)
				sendMessage(it->first, reply);
		}
	}

	void	broadcast(Channel *channel, const std::string &reply) const
	{
		std::map<Client*, bitMask>	members;

		if (!channel)
			return ;
		members = channel->getMembersMap();
		for (std::map<Client*, bitMask>::const_iterator it = members.begin();
			 it != members.end(); ++it)
			sendMessage(it->first, reply);
	}

	rfc	addToChannel(Client *client, const std::string &title,
				const std::string &pw)
	{
		if (!(client->getChannelsList().size() < MAX_CHANJOIN))
			return (irc::TOOMANYCHANNELS);
		if (!Channel::isTitleCompliant(title))
			return (irc::BADCHANMASK);
		if (!irc::isNameCompliant(pw))
			return (irc::BADCHANNELKEY);

		std::string	mapKey = Channel::title2key(title);
		Channel		*channel = _getChannel(mapKey);

		if (!channel)
		{
			channel = new Channel(title, pw);
			channels[mapKey] = channel;
			channel->addClient(client, US_FOUNDER | US_OPERATOR);
			client->addChannel(channel);
			return (irc::OK);
		}
		if (channel->isMember(client))
			return (irc::USERONCHANNEL);
		if (!channel->passwordMatch(pw))
			return (irc::BADCHANNELKEY);
		if (!channel->belowChannelLimit())
			return (irc::CHANNELISFULL);
		if (!channel->joinGranted(client))
			return (irc::INVITEONLYCHAN);

		channel->addClient(client, US_BASIC);
		client->addChannel(channel);
		return (irc::OK);
	}

	void	removeFromChannel(Client *client, const std::string &title,
				const std::string &)
	{
		std::string	mapKey = Channel::title2key(title);
		Channel		*channel = _getChannel(mapKey);

		if (!channel)
			return ;
		channel->removeClient(client);
		client->removeChannel(channel);
		if (channel->isEmpty())
			_removeChannel(mapKey);
	}

	Channel	*getChannelByTitle(std::string title) const
	{
		return (_getChannel(Channel::title2key(title)));
	}

private:
	Channel	*_getChannel(const std::string &mapKey) const
	{
		std::map<std::string, Channel*>::const_iterator	it;

		it = channels.find(mapKey);
		if (it == channels.end())
			return (0);
		return (it->second);
	}

	void	_removeChannel(const std::string &mapKey)
	{
		std::map<std::string, Channel*>::iterator	it;

		it = channels.find(mapKey);
		if (it == channels.end())
			return ;
		delete it->second;
		channels.erase(it);
	}
};

#endif
