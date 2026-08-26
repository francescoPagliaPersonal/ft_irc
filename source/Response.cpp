/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mweghofe <mweghofe@student.42vienna.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/26 10:51:34 by fpaglia          #+#    #+#             */
/*   Updated: 2026/08/26 15:51:42 by mweghofe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "Response.hpp"
#include "ft_irc.hpp"
#include "irc.hpp"
#include <sstream>

std::string Response::_server;
std::map<irc::rfc, std::string> Response::_numInfo;
std::map<irc::rfc, Response::type> Response::_numType;

/* The reply init currently lists only items that have a defined
   respond string.
   We could consider adding in the map all the other items that 
   have been defined as part of our numeric response world 
   and in case it's not found at all then throw an error.
   To be verified if it's meaningfull at all. 
 */
void Response::init(const std::string &srv)
{
	Response::_server = srv;
	
	// ------ REPLY CODES ------------------------------------------------------
	// _numInfo[irc::OK] = "";
	// _numInfo[irc::UMODEIS] = "";
	_numInfo[irc::AWAY] = ":I'm away.";
	// _numInfo[irc::CHANNELMODEIS] = "";
	// _numInfo[irc::CREATIONTIME] = "";
	_numInfo[irc::NOTOPIC] = ":No topic is set";
	// _numInfo[irc::TOPIC] = ":<topic>";
	// _numInfo[irc::TOPICWHOTIME] = "";
	// _numInfo[irc::INVITING] = "";
	// _numInfo[irc::NAMREPLY] = ":[prefix]<nick>{ [prefix]<nick>";
	_numInfo[irc::ENDOFNAMES] = ":End of /NAMES list";
	// _numInfo[irc::MOTD] = ":<line of the motd>";
	// _numInfo[irc::MOTDSTART] = ":- <server> Message of the day -";
	_numInfo[irc::ENDOFMOTD] = ":End of /MOTD command.";
	_numInfo[irc::YOUREOPER] = ":You are now an IRC operator";

	// ------ ERROR CODES ------------------------------------------------------
	_numInfo[irc::NOSUCHNICK] = ":No such nick.";
	_numInfo[irc::NOSUCHCHANNEL] = ":No such channel.";
	_numInfo[irc::CANNOTSENDTOCHAN] = ":Cannot send to channel.";
	_numInfo[irc::TOOMANYCHANNELS] = ":You have joined too many channels.";
	_numInfo[irc::NOORIGIN] = ":No origin specified.";
	_numInfo[irc::INVALIDCAPCMD] = "No such CAP command.";
	_numInfo[irc::NORECIPIENT] = ":No recipient given.";
	_numInfo[irc::NOTEXTTOSEND] = ":No text to send.";
	_numInfo[irc::UNKNOWNCOMMAND] = ":Unknown command.";
	_numInfo[irc::NONICKNAMEGIVEN] = ":No nickname given.";
	_numInfo[irc::ERRONEUSNICKNAME] = ":Erroneus nickname.";
	_numInfo[irc::NICKNAMEINUSE] = ":Nickname is already in use.";
	_numInfo[irc::USERNOTINCHANNEL] = ":They aren't on that channel.";
	_numInfo[irc::NOTONCHANNEL] = ":You're not on that channel.";
	_numInfo[irc::USERONCHANNEL] = ":is already on channel.";
	_numInfo[irc::NOTREGISTERED] = ":You have not registered";
	_numInfo[irc::NEEDMOREPARAMS] = ":Not enough parameters.";
	_numInfo[irc::ALREADYREGISTERED] = ":You may not reregister.";
	_numInfo[irc::PASSWDMISMATCH] = ":Password incorrect.";
	_numInfo[irc::CHANNELISFULL] = ":Cannot join channel (+l).";
	_numInfo[irc::UNKNOWNMODE] = ":is unknown mode char to me.";
	_numInfo[irc::INVITEONLYCHAN] = ":Cannot join channel (+i).";
	_numInfo[irc::BANNEDFROMCHAN] = ":Cannot join channel (+b).";
	_numInfo[irc::BADCHANNELKEY] = ":Cannot join channel (+k).";
	_numInfo[irc::BADCHANMASK] = ":Bad Channel Mask.";
	_numInfo[irc::CHANOPRIVSNEEDED] = ":You're not channel operator.";
	_numInfo[irc::NOOPERHOST] = ":No O-lines for your host.";
	_numInfo[irc::UMODEUNKNOWNFLAG] = ":Unknown MODE flag.";
	_numInfo[irc::USERSDONTMATCH] = ":Cant change mode for other users.";
	// _numInfo[irc::NOCONN] = "";
	_numInfo[irc::MANYPARAMS] = ":Too many parameter given";

	// ------ SPECIAL TYPES ----------------------------------------------------
	_numType[irc::NOSUCHNICK] = PARAM0;
	_numType[irc::NOSUCHCHANNEL] = PARAM0;
	_numType[irc::TOOMANYCHANNELS] = PARAM0;
	_numType[irc::INVALIDCAPCMD] = PARAM0;
	_numType[irc::ERRONEUSNICKNAME] = PARAM0;
	_numType[irc::NICKNAMEINUSE] = PARAM0;
	_numType[irc::USERONCHANNEL] = PARAM0;
	_numType[irc::UNKNOWNCOMMAND] = COMMAND;
	_numType[irc::NEEDMOREPARAMS] = COMMAND;
}

std::string Response::handleNumeric(const Message& msg, irc::rfc code)
{
	std::map<irc::rfc, type>::iterator it;
	it = _numType.find(code);
	if (it == _numType.end())
		return (noOpt(msg, code));
	switch (it->second)
	{
		case PARAM0:
			return (args(msg, code, msg.params[0]));
		case COMMAND:
			return (args(msg, code, msg.command));
	}
}

std::string Response::noOpt(const Message & msg, irc::rfc code)
{
	std::stringstream reply;
	std::map<irc::rfc, std::string>::const_iterator it;
	it = _numInfo.find(code);

	reply 
		<< ":" << Response::_server << " " 
		<< code << " "
		<< msg.sender->getNick() ;
	
	if (it != _numInfo.end())
		reply << " " << it->second;

	reply << CRLF;

	return reply.str();
}

std::string Response::args(const Message& msg, irc::rfc code, const std::string & args)
{
	std::stringstream reply;
	std::map<irc::rfc, std::string>::const_iterator it;
	it = _numInfo.find(code);

	reply 
		<< ":" << Response::_server << " " 
		<< code << " "
		<< msg.sender->getNick() << " "
		<< args;
	
	if (it != _numInfo.end())
		reply << " " << it->second;

	reply << CRLF;

	return reply.str();
}

std::string	Response::trailing(const Message& msg, irc::rfc code, const std::string & args, const std::string & trail)
{
	std::stringstream reply;

	reply 
		<< ":" << Response::_server << " " 
		<< code << " "
		<< msg.sender->getNick() << " "
		<< args << " "
		<< ":" << trail
		<< CRLF;

	return reply.str();
}

/* build a string in the form:
   :msg.sender->getID() COMMAND args :msg.trailing
   the response is split in 512 bytes if needed.
 */
std::string	Response::senderMessage(const Message& msg, const std::string & args)
{
	std::string reply;

	reply = ":" + msg.sender->getID() + " ";
	reply += msg.command + " " + args + " :";
	reply = irc::chunkifyTrailing(reply, msg.trailing);
	return reply;
}

/* build a string in the form:
   :msg.sender->getID() COMMAND args :custom trailing
   the response is split in 512 bytes if needed.
 */
std::string	Response::senderMessage(const Message& msg, const std::string & args, const std::string & trailing)
{
	std::string reply;

	reply = ":" + msg.sender->getID() + " ";
	reply += msg.command + " " + args + " :";
	reply = irc::chunkifyTrailing(reply, trailing);
	return reply;
}
