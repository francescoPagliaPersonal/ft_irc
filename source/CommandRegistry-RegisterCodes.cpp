/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CommandRegistry-RegisterCodes.cpp                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mweghofe <mweghofe@student.42vienna.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/18 12:35:10 by mweghofe          #+#    #+#             */
/*   Updated: 2026/08/20 09:24:15 by mweghofe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "CommandRegistry.hpp"
#include "RFCnumeric.hpp"
#include <sstream>

void CommandRegistry::registerCodes()
{
	_rfcCodes[rfc::UNKNOWNCOMMAND] = rfc::badCmd;
	_rfcCodes[rfc::NICKNAMEINUSE] = rfc::nickInUse;
	_rfcCodes[rfc::NOTREGISTERED] = rfc::notRegistered;
	_rfcCodes[rfc::ALREADYREGISTERED] = rfc::alreadyRegistered;
	_rfcCodes[rfc::NEEDMOREPARAMS] = rfc::tooFewParams;
	_rfcCodes[rfc::PASSWDMISMATCH] = rfc::badPassword;
	_rfcCodes[rfc::NONICKNAMEGIVEN] = rfc::noNick;
	_rfcCodes[rfc::ERRONEUSNICKNAME] = rfc::nickBad;
}
