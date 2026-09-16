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
#include <string>

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
	_numInfo[irc::NOSUCHSERVER] = ":No such server.";
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
	_numInfo[irc::KEYSET] = ":Channel key already set";
	_numInfo[irc::CHANNELISFULL] = ":Cannot join channel (+l).";
	_numInfo[irc::UNKNOWNMODE] = ":is unknown mode char to me.";
	_numInfo[irc::INVITEONLYCHAN] = ":Cannot join channel (+i).";
	_numInfo[irc::BANNEDFROMCHAN] = ":Cannot join channel (+b).";
	_numInfo[irc::BADCHANNELKEY] = ":Cannot join channel (+k).";
	_numInfo[irc::BADCHANMASK] = ":Bad Channel Mask.";
	_numInfo[irc::CHANOPRIVSNEEDED] = ":You're not channel operator.";
	_numInfo[irc::NOOPERHOST] = ":No O-lines for your host.";
	// _numInfo[irc::UMODEUNKNOWNFLAG] = ":Unknown MODE flag.";
	_numInfo[irc::UMODEUNKNOWNFLAG] = ":User MODE is not supported."; // custom
	_numInfo[irc::USERSDONTMATCH] = ":Cant change mode for other users.";
	_numInfo[irc::MANYPARAMS] = ":Too many parameter given";

	// ------ SPECIAL TYPES ----------------------------------------------------
	_numType[irc::NOSUCHNICK] = PARAM0;
	_numType[irc::NOSUCHSERVER] = PARAM0;
	_numType[irc::NOSUCHCHANNEL] = PARAM0;
	_numType[irc::TOOMANYCHANNELS] = PARAM0;
	_numType[irc::INVALIDCAPCMD] = PARAM0;
	_numType[irc::ERRONEUSNICKNAME] = PARAM0;
	_numType[irc::NICKNAMEINUSE] = PARAM0;
	_numType[irc::USERONCHANNEL] = PARAM0;
	_numType[irc::NOTONCHANNEL] = PARAM0;
	_numType[irc::CHANOPRIVSNEEDED] = PARAM0;
	_numType[irc::UNKNOWNCOMMAND] = COMMAND;
	_numType[irc::NEEDMOREPARAMS] = COMMAND;
}
std::string Response::getServerName()
{
	return _server;
}

/*
	Build the default prefix string for numeric reply:
	|:<serverName> <numericCode> <senderNick>|
*/
std::string Response::_buildNumericPrefix(const Message& msg, irc::rfc code)
{
	std::stringstream prefix;
	prefix
		<< ":" << Response::_server << " " 
		<< code << " "
		<< msg.sender->getNick();
	return (prefix.str());
}

/*
	Builds the correct string for a semi-automatic error reply numeric:
	- default is:
	:<serverName> CODE <senderNick> :<default trailing>
	- variant PARAM0
	:<serverName> CODE <senderNick> msg.params[0] :<default trailing>
	- variant COMMAND 
	:<serverName> CODE <senderNick> msg.command :<default trailing>
*/
std::string Response::handleNumeric(const Message& msg, irc::rfc code)
{
	std::map<irc::rfc, type>::iterator it;
	// SPECIAL CASE: command QUIT
	if (code == irc::HASQUIT)
		return (buildError(msg, "Closing link", "Quit"));
	// look up which method is needed for custom ARGS of the requested error
	it = _numType.find(code);
	// execute DEFAULT variant
	if (it == _numType.end())
		return (buildNumeric(msg, code));
	// execute PARAM0 and COMMAND
	switch (it->second)
	{
		case PARAM0:
			return (buildNumeric(msg, code, msg.params[0]));
		case COMMAND:
			return (buildNumeric(msg, code, msg.command));
	}
	return std::string("");
}

/*
	Build a string in the form:
	:<serverName> CODE <senderNick> :<default trailing>
	The response is split in 512 bytes if needed.
*/
std::string Response::buildNumeric(const Message & msg, irc::rfc code)
{
	// retrieve default trailing message for code
	std::map<irc::rfc, std::string>::const_iterator it;
	it = _numInfo.find(code);
	// build default prefix for numeric reply
	std::string reply(_buildNumericPrefix(msg, code));
	// append default trailing, if any
	if (it != _numInfo.end())
		reply.append(" " + it->second);
	// finish
	reply.append(CRLF);
	return (reply);
}

/*
	Build a string in the form:
	:<serverName> CODE <senderNick> ARGS :<default trailing>
	The response is split in 512 bytes if needed.
*/
std::string Response::buildNumeric(const Message& msg, irc::rfc code, const std::string & args)
{
	// retrieve default trailing message for code
	std::map<irc::rfc, std::string>::const_iterator it;
	it = _numInfo.find(code);
	// build default prefix for numeric reply
	std::string reply(_buildNumericPrefix(msg, code));
	// SPECIAL: append custom ARGS
	if (!args.empty())
		reply.append(" " + args);
	// append default trailing, if any
	if (it != _numInfo.end())
		reply.append(" " + it->second);
	// finish
	reply.append(CRLF);
	return (reply);
}

/*
	Build a string in the form:
	:<serverName> CODE <senderNick> ARGS :<custom TRAIL>
	   The response is split in 512 bytes if needed.
*/
std::string	Response::buildNumeric(const Message& msg, irc::rfc code, const std::string & args, const std::string & trail)
{
	// build default prefix for numeric reply
	std::string reply(_buildNumericPrefix(msg, code));
	// SPECIAL: append custom ARGS
	if (!args.empty())
		reply.append(" " + args);
	// SPECIAL: append custom ARGS
	if (!trail.empty())
		reply.append(" :" + trail);
	// TODO else " :" needed w/o trail?
	// finish
	reply.append(CRLF);
	return (reply);
}

/*
	Build a string in the form:
	:msg.sender->getID() COMMAND args :msg.getTrailing()
	The response is split in 512 bytes if needed.
*/
std::string	Response::buildRegular(const Message& msg, const std::string & args)
{
	std::string reply;
	// build default prefix for regular reply
	reply = ":" + msg.sender->getID() + " " + msg.command;
	// append custom args, then default trailing
	if (!args.empty())
		reply += " " + args;
	if (!msg.getTrailing().empty())
	{
		reply += " :";
		reply = irc::chunkifyTrailing(reply, msg.getTrailing());
	}
	else
		reply.append(CRLF);
	return (reply);
}

/*
	Build a string in the form:
	:msg.sender->getID() COMMAND args :custom trailing
	The response is split in 512 bytes if needed.
*/
std::string	Response::buildRegular(const Message& msg, const std::string & args, const std::string & trailing)
{
	std::string reply;
	// build default prefix for regular reply
	reply = ":" + msg.sender->getID() + " " + msg.command;
	// append custom args then custom trailing
	if (!args.empty())
		reply += " " + args;
	if (!trailing.empty())
	{
		reply += " :";
		reply = irc::chunkifyTrailing(reply, trailing);
	}
	else
		reply.append(CRLF);
	return (reply);
}

std::string	Response::buildError(const Message& msg,
								 const std::string& reason,
								 const std::string& origin)
{
	std::string reply("ERROR");
	Client* client = msg.sender;
	// HACK protection for us
	if (reason.empty() || origin.empty())
		throw std::logic_error("We must give a reason and origin of the Error.");
	reply.append(" :" + reason + ": (");
	reply.append(client->getUserName() + "@" + client->getHost() + ")");
	if (!msg.getTrailing().empty())
		reply.append(" [" + origin + ": "+ msg.getTrailing() + "]");
	reply.append(CRLF);
	return (reply);
}
