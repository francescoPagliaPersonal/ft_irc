/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   args.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mweghofe <mweghofe@student.42vienna.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/04 12:19:28 by fpaglia           #+#    #+#             */
/*   Updated: 2026/08/27 13:04:33 by mweghofe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ARGS_HPP
# define ARGS_HPP

# include <string>
# include <sstream>

unsigned short	arg2port(const char *port_str);
std::string		arg2password(const char *pw_str);

#endif
