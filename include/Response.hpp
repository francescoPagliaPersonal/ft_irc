/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mweghofe <mweghofe@student.42vienna.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/26 10:50:10 by fpaglia           #+#    #+#             */
/*   Updated: 2026/08/27 19:56:00 by mweghofe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef REPLY_HPP
# define REPLY_HPP

// -------------------------------------------------------------------------- //

# include <map>

# include "Message.hpp"
# include "irc.hpp"

// -------------------------------------------------------------------------- //

class Response 
{
	public:
		// ---- init to store all data in private attributes
		static void			init(const std::string &);
		// ---- build any NUMERIC response string
		static std::string	buildNumeric(const Message&, irc::rfc);
		static std::string	buildNumeric(const Message&, irc::rfc, const std::string & args);
		static std::string	buildNumeric(const Message&, irc::rfc, const std::string & args, const std::string & trail);
		// ---- build any REGULAR response string
		static std::string	buildRegular(const Message&, const std::string & args);
		static std::string	buildRegular(const Message&, const std::string & args, const std::string & trail);
		// ---- build special ERROR response string
		static std::string	buildError(const Message&, const std::string& reason, const std::string& origin);
		// ---- error handler for semi-automatic error replies
		static std::string  handleNumeric(const Message&, irc::rfc);

	private:
		// ---- custom type for handle semi-automatic error responses
		enum type
		{
			PARAM0,
			COMMAND
		};
		// ----
		//Just has a note this could have been the server itself but
		//since it's part of the srv constructor I kept it simpler
		static std::string		 				_server;
		static std::map<irc::rfc, std::string>	_numInfo;
		static std::map<irc::rfc, type>			_numType;
		// ----
		static std::string _buildNumericPrefix(const Message&, irc::rfc);
};

#endif //reply class
