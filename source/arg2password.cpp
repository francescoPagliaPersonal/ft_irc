/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   arg2password.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mweghofe <mweghofe@student.42vienna.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/04 12:22:04 by fpaglia           #+#    #+#             */
/*   Updated: 2026/08/17 17:41:18 by mweghofe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "args.hpp"
#include <stdexcept>

namespace {
bool contains_anumber(std::string & passwd)
{
	int i = 0;
	while (passwd[i] != '\0')
	{
		if (std::isdigit(passwd[i]))
			return true;
		++i;
	}
	return false;
}

bool contains_aletter(std::string & passwd)
{
	int i = 0;
	while (passwd[i] != '\0')
	{
		if (std::isalpha(passwd[i]))
			return true;
		++i;
	}
	return false;
}

bool contains_aSpecialChar(std::string & passwd)
{
	int i = 0;
	while (passwd[i] != '\0')
	{
		if (!std::isalnum(passwd[i]))
			return true;
		++i;
	}
	return false;
}
} // end of namespace

// Convert a password string, validating length and character classes.
std::string	arg2password(const char *passwd_str)
{
	std::string	passwd(passwd_str);
	if (passwd.size() < 4)
		throw std::out_of_range("Password too short - min lenght: 4 characters.");
	if (!contains_aletter(passwd))
		throw std::invalid_argument("Password must include letters.");
	if (!contains_anumber(passwd))
		throw std::invalid_argument("Password must include numbers.");
	if (!contains_aSpecialChar(passwd))
		throw std::invalid_argument("Password must include special characters.");
	return passwd;
}
