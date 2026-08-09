/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   args.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fpaglia <fpaglia@student.42vienna.com      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/04 12:19:28 by fpaglia           #+#    #+#             */
/*   Updated: 2026/08/04 12:28:46 by fpaglia          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ARGS_HPP
# define ARGS_HPP

#include <string>
#include <sstream>

int			arg2port(const char *port_str);
std::string	arg2password(const char *passwd_str);

#endif
