/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_mode.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mweghofe <mweghofe@student.42vienna.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/06 00:00:00 by mweghofe          #+#    #+#             */
/*   Updated: 2026/09/06 00:00:00 by mweghofe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "CommandRegistry.hpp"
#include "Message.hpp"
#include "client_fixture.hpp"
#include "fake_server.hpp"
#include "harness.hpp"
#include "test_cmd_helper.hpp"

#include <string>

TEST(mode_query_empty_channel)
{
	CommandRegistry	reg;
	FakeServer		srv;
	TestClient		alice;

	reg.registerCmds();
	registerClient(srv, alice, "alice");
	joinChannel(reg, srv, alice, "#chan");
	srv.sent.clear();
	reg.execute(srv, irc::string2Message("MODE #chan", &alice.client));
	CHECK(sentContains(srv, &alice.client, " 324 "));
	CHECK(sentContains(srv, &alice.client, "#chan +"));
}

TEST(mode_query_non_member_allowed)
{
	CommandRegistry	reg;
	FakeServer		srv;
	TestClient		alice;
	TestClient		bob;

	reg.registerCmds();
	registerClient(srv, alice, "alice");
	registerClient(srv, bob, "bob");
	joinChannel(reg, srv, alice, "#chan");
	srv.sent.clear();
	reg.execute(srv, irc::string2Message("MODE #chan", &bob.client));
	CHECK(sentContains(srv, &bob.client, " 324 "));
}

TEST(mode_no_such_channel)
{
	CommandRegistry	reg;
	FakeServer		srv;
	TestClient		alice;

	reg.registerCmds();
	registerClient(srv, alice, "alice");
	reg.execute(srv, irc::string2Message("MODE #nope +i", &alice.client));
	CHECK_EQ(lastTo(srv, &alice.client),
		std::string(":CoolServ 403 alice #nope :No such channel.\r\n"));
}

TEST(mode_not_on_channel)
{
	CommandRegistry	reg;
	FakeServer		srv;
	TestClient		alice;
	TestClient		bob;

	reg.registerCmds();
	registerClient(srv, alice, "alice");
	registerClient(srv, bob, "bob");
	joinChannel(reg, srv, alice, "#chan");
	srv.sent.clear();
	reg.execute(srv, irc::string2Message("MODE #chan +i", &bob.client));
	CHECK_EQ(lastTo(srv, &bob.client),
		std::string(":CoolServ 442 bob #chan :You're not on that channel.\r\n"));
}

TEST(mode_non_op)
{
	CommandRegistry	reg;
	FakeServer		srv;
	TestClient		alice;
	TestClient		bob;

	reg.registerCmds();
	registerClient(srv, alice, "alice");
	registerClient(srv, bob, "bob");
	joinChannel(reg, srv, alice, "#chan");
	joinChannel(reg, srv, bob, "#chan");
	srv.sent.clear();
	reg.execute(srv, irc::string2Message("MODE #chan +i", &bob.client));
	CHECK_EQ(lastTo(srv, &bob.client),
		std::string(":CoolServ 482 bob #chan :You're not channel operator.\r\n"));
}

TEST(mode_set_invite_broadcasts)
{
	CommandRegistry	reg;
	FakeServer		srv;
	TestClient		alice;
	TestClient		bob;

	reg.registerCmds();
	registerClient(srv, alice, "alice");
	registerClient(srv, bob, "bob");
	joinChannel(reg, srv, alice, "#chan");
	joinChannel(reg, srv, bob, "#chan");
	srv.sent.clear();
	reg.execute(srv, irc::string2Message("MODE #chan +i", &alice.client));
	CHECK(sentContains(srv, &bob.client, " MODE #chan +i"));
	CHECK(srv.getChannelByTitle("#chan")->getModes() & CH_INVITE);
}

TEST(mode_key_blocks_join)
{
	CommandRegistry	reg;
	FakeServer		srv;
	TestClient		alice;
	TestClient		bob;

	reg.registerCmds();
	registerClient(srv, alice, "alice");
	registerClient(srv, bob, "bob");
	joinChannel(reg, srv, alice, "#chan");
	srv.sent.clear();
	reg.execute(srv, irc::string2Message("MODE #chan +k secret", &alice.client));
	srv.sent.clear();
	reg.execute(srv, irc::string2Message("JOIN #chan wrong", &bob.client));
	CHECK_EQ(lastTo(srv, &bob.client),
		std::string(":CoolServ 475 bob #chan :Cannot join channel (+k).\r\n"));
}

TEST(mode_limit_full)
{
	CommandRegistry	reg;
	FakeServer		srv;
	TestClient		alice;
	TestClient		bob;
	TestClient		carol;

	reg.registerCmds();
	registerClient(srv, alice, "alice");
	registerClient(srv, bob, "bob");
	registerClient(srv, carol, "carol");
	joinChannel(reg, srv, alice, "#chan");
	srv.sent.clear();
	reg.execute(srv, irc::string2Message("MODE #chan +l 2", &alice.client));
	joinChannel(reg, srv, bob, "#chan");
	srv.sent.clear();
	reg.execute(srv, irc::string2Message("JOIN #chan", &carol.client));
	CHECK_EQ(lastTo(srv, &carol.client),
		std::string(":CoolServ 471 carol #chan :Cannot join channel (+l).\r\n"));
}

TEST(mode_give_operator)
{
	CommandRegistry	reg;
	FakeServer		srv;
	TestClient		alice;
	TestClient		bob;

	reg.registerCmds();
	registerClient(srv, alice, "alice");
	registerClient(srv, bob, "bob");
	joinChannel(reg, srv, alice, "#chan");
	joinChannel(reg, srv, bob, "#chan");
	srv.sent.clear();
	reg.execute(srv, irc::string2Message("MODE #chan +o bob", &alice.client));
	CHECK(srv.getChannelByTitle("#chan")->isChanOp(&bob.client));
	CHECK(sentContains(srv, &bob.client, " MODE #chan +o bob"));
}

TEST(mode_unknown_letter)
{
	CommandRegistry	reg;
	FakeServer		srv;
	TestClient		alice;

	reg.registerCmds();
	registerClient(srv, alice, "alice");
	joinChannel(reg, srv, alice, "#chan");
	srv.sent.clear();
	reg.execute(srv, irc::string2Message("MODE #chan +z", &alice.client));
	CHECK(sentContains(srv, &alice.client, " 472 "));
}

TEST(mode_remove_invite)
{
	CommandRegistry	reg;
	FakeServer		srv;
	TestClient		alice;

	reg.registerCmds();
	registerClient(srv, alice, "alice");
	joinChannel(reg, srv, alice, "#chan");
	reg.execute(srv, irc::string2Message("MODE #chan +i", &alice.client));
	srv.sent.clear();
	reg.execute(srv, irc::string2Message("MODE #chan -i", &alice.client));
	CHECK(!(srv.getChannelByTitle("#chan")->getModes() & CH_INVITE));
	CHECK(sentContains(srv, &alice.client, " MODE #chan -i"));
}

TEST(mode_remove_topic_flag)
{
	CommandRegistry	reg;
	FakeServer		srv;
	TestClient		alice;

	reg.registerCmds();
	registerClient(srv, alice, "alice");
	joinChannel(reg, srv, alice, "#chan");
	reg.execute(srv, irc::string2Message("MODE #chan +t", &alice.client));
	srv.sent.clear();
	reg.execute(srv, irc::string2Message("MODE #chan -t", &alice.client));
	CHECK(!(srv.getChannelByTitle("#chan")->getModes() & CH_TOPIC));
}

TEST(mode_remove_limit)
{
	CommandRegistry	reg;
	FakeServer		srv;
	TestClient		alice;
	TestClient		bob;

	reg.registerCmds();
	registerClient(srv, alice, "alice");
	registerClient(srv, bob, "bob");
	joinChannel(reg, srv, alice, "#chan");
	reg.execute(srv, irc::string2Message("MODE #chan +l 2", &alice.client));
	joinChannel(reg, srv, bob, "#chan");
	srv.sent.clear();
	reg.execute(srv, irc::string2Message("MODE #chan -l", &alice.client));
	CHECK(!(srv.getChannelByTitle("#chan")->getModes() & CH_LIMIT));
	reg.execute(srv, irc::string2Message("JOIN #chan", &bob.client));
	CHECK(!sentContains(srv, &bob.client, " 471 "));
}

TEST(mode_remove_key_with_matching_arg)
{
	CommandRegistry	reg;
	FakeServer		srv;
	TestClient		alice;
	TestClient		bob;

	reg.registerCmds();
	registerClient(srv, alice, "alice");
	registerClient(srv, bob, "bob");
	joinChannel(reg, srv, alice, "#chan");
	reg.execute(srv, irc::string2Message("MODE #chan +k secret", &alice.client));
	srv.sent.clear();
	reg.execute(srv, irc::string2Message("MODE #chan -k secret", &alice.client));
	CHECK(!(srv.getChannelByTitle("#chan")->getModes() & CH_PASSWORD));
	srv.sent.clear();
	reg.execute(srv, irc::string2Message("JOIN #chan", &bob.client));
	CHECK(!sentContains(srv, &bob.client, " 475 "));
}

TEST(mode_remove_key_without_arg)
{
	CommandRegistry	reg;
	FakeServer		srv;
	TestClient		alice;

	reg.registerCmds();
	registerClient(srv, alice, "alice");
	joinChannel(reg, srv, alice, "#chan");
	reg.execute(srv, irc::string2Message("MODE #chan +k secret", &alice.client));
	srv.sent.clear();
	reg.execute(srv, irc::string2Message("MODE #chan -k", &alice.client));
	CHECK_EQ(lastTo(srv, &alice.client),
		std::string(":CoolServ 461 alice MODE #chan :Not enough parameters.\r\n"));
}

TEST(mode_combined_iktl_query)
{
	CommandRegistry	reg;
	FakeServer		srv;
	TestClient		alice;

	reg.registerCmds();
	registerClient(srv, alice, "alice");
	joinChannel(reg, srv, alice, "#chan");
	reg.execute(srv, irc::string2Message("MODE #chan +iktl secret 10",
			&alice.client));
	srv.sent.clear();
	reg.execute(srv, irc::string2Message("MODE #chan", &alice.client));
	CHECK_EQ(lastTo(srv, &alice.client),
		std::string(":CoolServ 324 alice #chan +itkl secret :10\r\n"));
}

TEST(mode_iktol_value_stealing)
{
	CommandRegistry	reg;
	FakeServer		srv;
	TestClient		alice;
	TestClient		bob;

	reg.registerCmds();
	registerClient(srv, alice, "alice");
	registerClient(srv, bob, "bob");
	joinChannel(reg, srv, alice, "#chan");
	srv.sent.clear();
	reg.execute(srv, irc::string2Message("MODE #chan +iktol secret 10",
			&alice.client));
	srv.sent.clear();
	reg.execute(srv, irc::string2Message("JOIN #chan wrong", &bob.client));
	CHECK_EQ(lastTo(srv, &bob.client),
		std::string(":CoolServ 475 bob #chan :Cannot join channel (+k).\r\n"));
}

TEST(mode_second_key_returns_keyset)
{
	CommandRegistry	reg;
	FakeServer		srv;
	TestClient		alice;

	reg.registerCmds();
	registerClient(srv, alice, "alice");
	joinChannel(reg, srv, alice, "#chan");
	reg.execute(srv, irc::string2Message("MODE #chan +k first", &alice.client));
	srv.sent.clear();
	reg.execute(srv, irc::string2Message("MODE #chan +k second", &alice.client));
	CHECK_EQ(lastTo(srv, &alice.client),
		std::string(":CoolServ 467 alice :Channel key already set\r\n"));
}

TEST(mode_user_mode_stub)
{
	CommandRegistry	reg;
	FakeServer		srv;
	TestClient		alice;

	reg.registerCmds();
	registerClient(srv, alice, "alice");
	reg.execute(srv, irc::string2Message("MODE alice +i", &alice.client));
	CHECK_EQ(lastTo(srv, &alice.client),
		std::string(":CoolServ 221 alice :not supported\r\n"));
}

TEST(mode_users_dont_match)
{
	CommandRegistry	reg;
	FakeServer		srv;
	TestClient		alice;
	TestClient		bob;

	reg.registerCmds();
	registerClient(srv, alice, "alice");
	registerClient(srv, bob, "bob");
	reg.execute(srv, irc::string2Message("MODE bob +i", &alice.client));
	CHECK_EQ(lastTo(srv, &alice.client),
		std::string(":CoolServ 502 alice :Cant change mode for other users.\r\n"));
}

TEST(mode_bad_first_char)
{
	CommandRegistry	reg;
	FakeServer		srv;
	TestClient		alice;

	reg.registerCmds();
	registerClient(srv, alice, "alice");
	joinChannel(reg, srv, alice, "#chan");
	srv.sent.clear();
	reg.execute(srv, irc::string2Message("MODE #chan z", &alice.client));
	CHECK(sentContains(srv, &alice.client, " 472 "));
	CHECK(!sentContains(srv, &alice.client, " MODE #chan"));
}
