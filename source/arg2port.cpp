/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   arg2port.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mweghofe <mweghofe@student.42vienna.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/04 12:25:05 by fpaglia           #+#    #+#             */
/*   Updated: 2026/08/09 21:13:16 by mweghofe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "args.hpp"

unsigned short	arg2port(const char *port_str)
{
	std::istringstream	stream(port_str);
	int					num;
	
	stream >> num;
	if (stream.fail() == true) // ie. >> fails if there is no number
		throw std::invalid_argument("Port input data is invalid.");
	std::string leftover;
	stream >> leftover;
	if (stream.fail() == false) // ie. >> succeeds if garbage remains
		throw std::invalid_argument("Port input data is invalid.");
	if (num < 1024 || num > 65535)
		throw std::out_of_range("Port number must be between 1024 and 65535.");
	return (static_cast<unsigned short>(num));
}
