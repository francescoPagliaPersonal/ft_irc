/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fpaglia <fpaglia@student.42vienna.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/26 10:50:10 by fpaglia           #+#    #+#             */
/*   Updated: 2026/08/26 14:23:32 by fpaglia          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef REPLY_HPP
# define REPLY_HPP

#include "Message.hpp"
# include "irc.hpp"

class Response 
{
	public:
	static void			init(const std::string &);
	static std::string	noOpt(const Message&, irc::rfc);
	static std::string	args(const Message&, irc::rfc, const std::string & args);
	static std::string	trailing(const Message&, irc::rfc, const std::string & args, const std::string & trail);
	static std::string	senderMessage(const Message&, const std::string & args);
	static std::string	senderMessage(const Message&, const std::string & args, const std::string & trail);
	
	private:
	//Just has a note this could have been the server itself but
	//since it's part of the srv constructor I kept it simpler
	static std::string		 				_server;
	static std::map<irc::rfc, std::string>	_numInfo;	
	
};

#endif //reply class
