/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   arg2ip.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mweghofe <mweghofe@student.42vienna.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/02 09:00:44 by mweghofe          #+#    #+#             */
/*   Updated: 2026/09/02 09:31:55 by mweghofe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "args.hpp"

#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

#include <cstring>
#include <stdexcept>
#include <string>

// Convert a hostname or IPv4 string to network-order uint32 for sin_addr.s_addr.
unsigned int	arg2ip(const char *ip_str)
{
	if (ip_str == NULL || ip_str[0] == '\0')
		throw std::invalid_argument("IP/host string is empty.");

	struct addrinfo	hints;
	struct addrinfo	*res;
	// 1) set what the address is supposed to look like
	std::memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	// 2) take the string and check it for a valid address
	int ret = ::getaddrinfo(ip_str, NULL, &hints, &res);
	if (ret != 0)
		throw std::invalid_argument(
			std::string("Invalid IP/host: ") + ::gai_strerror(ret));
	// 3) extract the found ip address from ai_addr
	struct sockaddr_in	*addr;
	addr = reinterpret_cast<struct sockaddr_in *>(res->ai_addr);
	unsigned int ip = addr->sin_addr.s_addr;
	::freeaddrinfo(res);
	return (ip);
}
