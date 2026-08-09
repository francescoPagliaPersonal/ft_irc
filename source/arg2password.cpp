/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   arg2password.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fpaglia <fpaglia@student.42vienna.com      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/04 12:22:04 by fpaglia           #+#    #+#             */
/*   Updated: 2026/08/04 12:24:52 by fpaglia          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "args.hpp"

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

std::string	arg2password(const char *passwd_str)
{
	std::string	passwd(passwd_str);
	if (passwd.size() < 4)
		throw std::runtime_error("password too short - min lenght: 4 characters.");
	if (!contains_aletter(passwd))
		throw std::runtime_error("password must include letters.");
	if (!contains_anumber(passwd))
		throw std::runtime_error("password must include numbers.");
	if (!contains_aSpecialChar(passwd))
		throw std::runtime_error("password must include special characters.");
	return passwd;
}
