/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_message.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mweghofe <mweghofe@student.42vienna.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/24 10:48:00 by mweghofe          #+#    #+#             */
/*   Updated: 2026/08/31 09:29:58 by mweghofe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Message.hpp"
#include "client_fixture.hpp"
#include "harness.hpp"

#include <string>

TEST(message_nick_simple)
{
	TestClient	tc;
	Message		msg = irc::string2Message("NICK alice", &tc.client);

	CHECK_EQ(msg.sender, &tc.client);
	CHECK(msg.flags & irc::MSG_HAS_COMMAND);
	CHECK(msg.flags & irc::MSG_HAS_PARAMS);
	CHECK(!(msg.flags & irc::MSG_HAS_PREFIX));
	CHECK(!(msg.flags & irc::MSG_HAS_TRAILING));
	CHECK_EQ(msg.command, std::string("NICK"));
	CHECK_EQ(msg.params.size(), 1u);
	CHECK_EQ(msg.params[0], std::string("alice"));
	CHECK_EQ(irc::argCount(msg), 1);
}

TEST(message_privmsg_trailing)
{
	TestClient	tc;
	Message		msg = irc::string2Message("PRIVMSG bob :hello there", &tc.client);

	CHECK(msg.flags & irc::MSG_HAS_COMMAND);
	CHECK(msg.flags & irc::MSG_HAS_PARAMS);
	CHECK(msg.flags & irc::MSG_HAS_TRAILING);
	CHECK_EQ(msg.command, std::string("PRIVMSG"));
	CHECK_EQ(msg.params.size(), 1u);
	CHECK_EQ(msg.params[0], std::string("bob"));
	CHECK_EQ(msg.trailing, std::string("hello there"));
	CHECK_EQ(irc::argCount(msg), 2);
}

TEST(message_trims_spaces)
{
	TestClient	tc;
	Message		msg = irc::string2Message("  NICK   alice  ", &tc.client);

	CHECK_EQ(msg.command, std::string("NICK"));
	CHECK_EQ(msg.params.size(), 1u);
	CHECK_EQ(msg.params[0], std::string("alice"));
}

TEST(message_empty_has_no_command)
{
	TestClient	tc;
	Message		empty = irc::string2Message("", &tc.client);
	Message		spaces = irc::string2Message("   ", &tc.client);

	CHECK_EQ(empty.flags, 0);
	CHECK_EQ(spaces.flags, 0);
	CHECK(!(empty.flags & irc::MSG_HAS_COMMAND));
	CHECK(!(spaces.flags & irc::MSG_HAS_COMMAND));
	CHECK_EQ(irc::argCount(empty), 0);
}

TEST(message_prefix_and_command)
{
	TestClient	tc;
	Message		msg = irc::string2Message(":nick CMD arg", &tc.client);

	CHECK(msg.flags & irc::MSG_HAS_PREFIX);
	CHECK(msg.flags & irc::MSG_HAS_COMMAND);
	CHECK(msg.flags & irc::MSG_HAS_PARAMS);
	CHECK_EQ(msg.prefix, std::string("nick"));
	CHECK_EQ(msg.command, std::string("CMD"));
	CHECK_EQ(msg.params.size(), 1u);
	CHECK_EQ(msg.params[0], std::string("arg"));
}

TEST(message_prefix_only)
{
	TestClient	tc;
	Message		msg = irc::string2Message(":nick", &tc.client);

	CHECK(msg.flags & irc::MSG_HAS_PREFIX);
	CHECK(!(msg.flags & irc::MSG_HAS_COMMAND));
	CHECK_EQ(msg.prefix, std::string("nick"));
	CHECK(msg.command.empty());
}

TEST(message_empty_trailing_not_flagged)
{
	TestClient	tc;
	Message		msg = irc::string2Message("PRIVMSG bob :", &tc.client);

	CHECK(msg.flags & irc::MSG_HAS_PARAMS);
	CHECK(!(msg.flags & irc::MSG_HAS_TRAILING));
	CHECK(msg.trailing.empty());
	CHECK_EQ(irc::argCount(msg), 1);
}

TEST(message_colon_inside_param_is_not_trailing)
{
	TestClient	tc;
	Message		msg = irc::string2Message("PRIVMSG bob:here hello", &tc.client);

	CHECK(!(msg.flags & irc::MSG_HAS_TRAILING));
	CHECK_EQ(msg.params.size(), 2u);
	CHECK_EQ(msg.params[0], std::string("bob:here"));
	CHECK_EQ(msg.params[1], std::string("hello"));
}

TEST(message_trailing_keeps_inner_colon)
{
	TestClient	tc;
	Message		msg = irc::string2Message("PRIVMSG bob :hello :world", &tc.client);

	CHECK(msg.flags & irc::MSG_HAS_TRAILING);
	CHECK_EQ(msg.trailing, std::string("hello :world"));
}

TEST(message_command_case_folded)
{
	TestClient	tc;
	Message		lower = irc::string2Message("nick alice", &tc.client);
	Message		mixed = irc::string2Message("PrivMsg bob :hi", &tc.client);

	CHECK_EQ(lower.command, std::string("NICK"));
	CHECK_EQ(mixed.command, std::string("PRIVMSG"));
}

TEST(message_several_params)
{
	TestClient	tc;
	Message		msg = irc::string2Message("MODE #c +o bob", &tc.client);

	CHECK_EQ(msg.command, std::string("MODE"));
	CHECK_EQ(msg.params.size(), 3u);
	CHECK_EQ(msg.params[0], std::string("#c"));
	CHECK_EQ(msg.params[1], std::string("+o"));
	CHECK_EQ(msg.params[2], std::string("bob"));
	CHECK_EQ(irc::argCount(msg), 3);
}

TEST(message_user_four_args)
{
	TestClient	tc;
	Message		msg = irc::string2Message("USER ident 0 * :real name", &tc.client);

	CHECK_EQ(msg.command, std::string("USER"));
	CHECK_EQ(msg.params.size(), 3u);
	CHECK_EQ(msg.params[0], std::string("ident"));
	CHECK_EQ(msg.params[1], std::string("0"));
	CHECK_EQ(msg.params[2], std::string("*"));
	CHECK_EQ(msg.trailing, std::string("real name"));
	CHECK_EQ(irc::argCount(msg), 4);
}

TEST(message_argcount_trailing_only)
{
	TestClient	tc;
	Message		msg = irc::string2Message("PING :lag", &tc.client);

	CHECK(msg.flags & irc::MSG_HAS_TRAILING);
	CHECK(!(msg.flags & irc::MSG_HAS_PARAMS));
	CHECK_EQ(msg.trailing, std::string("lag"));
	CHECK_EQ(irc::argCount(msg), 1);
}
