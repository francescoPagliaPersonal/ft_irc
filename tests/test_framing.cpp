/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_framing.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mweghofe <mweghofe@student.42vienna.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/31 11:00:00 by mweghofe          #+#    #+#             */
/*   Updated: 2026/08/31 11:00:00 by mweghofe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "CommandRegistry.hpp"
#include "Message.hpp"
#include "fake_server.hpp"
#include "harness.hpp"
#include "socket_client_fixture.hpp"
#include "test_cmd_helper.hpp"

#include <string>
#include <vector>

namespace
{
	std::string	repeatChar(char ch, size_t n)
	{
		return (std::string(n, ch));
	}
}

TEST(framing_two_complete_lines)
{
	SocketTestClient			stc;
	std::vector<std::string>	msgs;

	msgs = stc.feedRecvSplit("NICK a\r\nUSER b 0 * :r\r\n");
	CHECK_EQ(msgs.size(), 2u);
	CHECK_EQ(msgs[0], std::string("NICK a"));
	CHECK_EQ(msgs[1], std::string("USER b 0 * :r"));
}

TEST(framing_subject_partial_send)
{
	SocketTestClient			stc;
	std::vector<std::string>	msgs;

	msgs = stc.feedRecvSplit("com");
	CHECK(msgs.empty());
	msgs = stc.feedRecvSplit("man");
	CHECK(msgs.empty());
	msgs = stc.feedRecvSplit("d\r\n");
	CHECK_EQ(msgs.size(), 1u);
	CHECK_EQ(msgs[0], std::string("command"));
}

TEST(framing_leftover_then_complete)
{
	SocketTestClient			stc;
	std::vector<std::string>	msgs;

	msgs = stc.feedRecvSplit("PING lag");
	CHECK(msgs.empty());
	msgs = stc.feedRecvSplit("\r\n");
	CHECK_EQ(msgs.size(), 1u);
	CHECK_EQ(msgs[0], std::string("PING lag"));
}

TEST(framing_two_messages_second_incomplete)
{
	SocketTestClient			stc;
	std::vector<std::string>	msgs;

	msgs = stc.feedRecvSplit("PING a\r\nPING b");
	CHECK_EQ(msgs.size(), 1u);
	CHECK_EQ(msgs[0], std::string("PING a"));
	msgs = stc.feedRecvSplit("\r\n");
	CHECK_EQ(msgs.size(), 1u);
	CHECK_EQ(msgs[0], std::string("PING b"));
}

TEST(framing_lf_is_not_delimiter)
{
	SocketTestClient			stc;
	std::vector<std::string>	msgs;

	msgs = stc.feedRecvSplit("NICK a\n");
	CHECK(msgs.empty());
	msgs = stc.feedRecvSplit("\r\n");
	CHECK_EQ(msgs.size(), 1u);
	CHECK_EQ(msgs[0], std::string("NICK a\n"));
}

TEST(framing_oversized_leftover)
{
	SocketTestClient			stc;
	std::vector<std::string>	msgs;

	msgs = stc.feedRecvSplit(repeatChar('x', 513));
	CHECK_EQ(msgs.size(), 1u);
	CHECK_EQ(msgs[0].size(), 513u);
}

TEST(framing_512_leftover_not_emitted)
{
	SocketTestClient			stc;
	std::vector<std::string>	msgs;

	msgs = stc.feedRecvSplit(repeatChar('x', 512));
	CHECK(msgs.empty());
}

TEST(framing_crlf_only)
{
	SocketTestClient			stc;
	std::vector<std::string>	msgs;

	msgs = stc.feedRecvSplit("\r\n");
	CHECK_EQ(msgs.size(), 1u);
	CHECK_EQ(msgs[0], std::string(""));
}

TEST(framing_oversized_clears_leftover)
{
	SocketTestClient			stc;
	std::vector<std::string>	msgs;

	msgs = stc.feedRecvSplit(repeatChar('x', 513));
	CHECK_EQ(msgs.size(), 1u);
	msgs = stc.feedRecvSplit("PING a\r\n");
	CHECK_EQ(msgs.size(), 1u);
	CHECK_EQ(msgs[0], std::string("PING a"));
}

TEST(framing_double_crlf)
{
	SocketTestClient			stc;
	std::vector<std::string>	msgs;

	msgs = stc.feedRecvSplit("PING a\r\n\r\n");
	CHECK_EQ(msgs.size(), 2u);
	CHECK_EQ(msgs[0], std::string("PING a"));
	CHECK_EQ(msgs[1], std::string(""));
}

TEST(framing_cr_without_lf_is_leftover)
{
	SocketTestClient			stc;
	std::vector<std::string>	msgs;

	msgs = stc.feedRecvSplit("PING a\r");
	CHECK(msgs.empty());
	msgs = stc.feedRecvSplit("\n");
	CHECK_EQ(msgs.size(), 1u);
	CHECK_EQ(msgs[0], std::string("PING a"));
}

TEST(framing_512_with_crlf_is_one_message)
{
	SocketTestClient			stc;
	std::vector<std::string>	msgs;
	std::string					payload;

	payload = repeatChar('x', 510) + "\r\n";
	msgs = stc.feedRecvSplit(payload);
	CHECK_EQ(msgs.size(), 1u);
	CHECK_EQ(msgs[0].size(), 510u);
}

TEST(framing_partial_through_registry)
{
	SocketTestClient			stc;
	CommandRegistry				reg;
	FakeServer					srv;
	std::vector<std::string>	msgs;

	reg.registerCmds();
	msgs = stc.feedRecvSplit("com");
	CHECK(msgs.empty());
	msgs = stc.feedRecvSplit("man");
	CHECK(msgs.empty());
	msgs = stc.feedRecvSplit("d\r\n");
	CHECK_EQ(msgs.size(), 1u);
	CHECK_EQ(msgs[0], std::string("command"));
	reg.execute(srv, irc::string2Message(msgs[0], stc.client));
	CHECK_EQ(lastTo(srv, stc.client),
		std::string(":CoolServ 421 * COMMAND :Unknown command.\r\n"));
}
