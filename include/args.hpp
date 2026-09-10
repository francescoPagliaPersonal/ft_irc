/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   args.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fpaglia <fpaglia@student.42vienna.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/04 12:19:28 by fpaglia           #+#    #+#             */
/*   Updated: 2026/09/10 12:02:18 by fpaglia          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ARGS_HPP
# define ARGS_HPP

# include "ft_irc.hpp"
# include <string>
# include <sstream>

unsigned short	arg2port(const char *port_str);
std::string		arg2password(const char *pw_str);

# ifdef BONUS
unsigned int	arg2ip(const char *ip_str);
# endif

#endif
