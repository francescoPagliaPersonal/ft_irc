/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   arg2port.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fpaglia <fpaglia@student.42vienna.com      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/04 12:25:05 by fpaglia           #+#    #+#             */
/*   Updated: 2026/08/04 12:25:30 by fpaglia          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "args.hpp"

int	arg2port(const char *port_str)
{
	std::stringstream	stream(port_str);
	int					port;
	stream >> port;

	if ( stream.fail() || !stream.eof() )
		throw std::runtime_error("port input is incorrect.");
	if ( port < 1024 || port > 65535 )
		throw std::runtime_error("port input is not in range 1024 - 65535.");
	return port;
}
