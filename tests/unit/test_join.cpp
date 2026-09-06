/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_join.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mweghofe <mweghofe@student.42vienna.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/31 10:33:00 by mweghofe          #+#    #+#             */
/*   Updated: 2026/08/31 11:20:00 by mweghofe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "CommandRegistry.hpp"
#include "Message.hpp"
#include "client_fixture.hpp"
#include "fake_server.hpp"
#include "harness.hpp"
#include "test_cmd_helper.hpp"

#include <string>

TEST(join_requires_registration)
{
	CommandRegistry	reg;
	FakeServer		srv;
	TestClient		tc;

	reg.registerCmds();
	reg.execute(srv, irc::string2Message("JOIN #chan", &tc.client));
	CHECK_EQ(lastTo(srv, &tc.client),
		std::string(":CoolServ 451 * :You have not registered\r\n"));
	CHECK(srv.getChannelByTitle("#chan") == 0);
}

TEST(join_bad_title)
{
	CommandRegistry	reg;
	FakeServer		srv;
	TestClient		tc;

	reg.registerCmds();
	registerClient(srv, tc, "alice");
	reg.execute(srv, irc::string2Message("JOIN #ab", &tc.client));
	CHECK_EQ(lastTo(srv, &tc.client),
		std::string(":CoolServ 476 alice #ab :Bad Channel Mask.\r\n"));
}

TEST(join_create_channel)
{
	CommandRegistry	reg;
	FakeServer		srv;
	TestClient		tc;

	reg.registerCmds();
	registerClient(srv, tc, "alice");
	reg.execute(srv, irc::string2Message("JOIN #chan", &tc.client));
	Channel	*ch = srv.getChannelByTitle("#chan");

	CHECK(ch != 0);
	CHECK(ch->isMember(&tc.client));
	CHECK(ch->isChanOp(&tc.client));
	CHECK(sentContains(srv, &tc.client, " JOIN #chan"));
	CHECK(sentContains(srv, &tc.client, " 331 "));
	CHECK(!sentContains(srv, &tc.client, " 332 "));
	CHECK(sentContains(srv, &tc.client, " 353 "));
	CHECK(sentContains(srv, &tc.client, "@alice"));
	CHECK(sentContains(srv, &tc.client, " 366 "));
}

TEST(join_create_channel_exact_replies)
{
	CommandRegistry	reg;
	FakeServer		srv;
	TestClient		tc;

	reg.registerCmds();
	registerClient(srv, tc, "alice");
	reg.execute(srv, irc::string2Message("JOIN #chan", &tc.client));
	CHECK(sentContains(srv, &tc.client,
		":alice!user@0.0.0.0 JOIN #chan\r\n"));
	CHECK(sentContains(srv, &tc.client,
		":CoolServ 331 alice #chan :No topic is set\r\n"));
	CHECK(sentContains(srv, &tc.client,
		":CoolServ 353 alice = #chan :@alice \r\n"));
	CHECK(sentContains(srv, &tc.client,
		":CoolServ 366 alice #chan :End of /NAMES list\r\n"));
}

TEST(join_second_client_broadcast)
{
	CommandRegistry	reg;
	FakeServer		srv;
	TestClient		alice;
	TestClient		bob;

	reg.registerCmds();
	registerClient(srv, alice, "alice");
	registerClient(srv, bob, "bob");
	reg.execute(srv, irc::string2Message("JOIN #chan", &alice.client));
	srv.sent.clear();
	reg.execute(srv, irc::string2Message("JOIN #chan", &bob.client));
	CHECK(sentContains(srv, &alice.client, " JOIN #chan"));
	CHECK(sentContains(srv, &bob.client, " JOIN #chan"));
	CHECK(sentContains(srv, &bob.client, " 331 "));
	CHECK(!sentContains(srv, &bob.client, " 332 "));
	CHECK(sentContains(srv, &bob.client, " 353 "));
	CHECK(sentContains(srv, &bob.client, " 366 "));
}

TEST(join_duplicate)
{
	CommandRegistry	reg;
	FakeServer		srv;
	TestClient		tc;

	reg.registerCmds();
	registerClient(srv, tc, "alice");
	reg.execute(srv, irc::string2Message("JOIN #chan", &tc.client));
	srv.sent.clear();
	reg.execute(srv, irc::string2Message("JOIN #chan", &tc.client));
	CHECK_EQ(lastTo(srv, &tc.client),
		std::string(":CoolServ 443 alice #chan :is already on channel.\r\n"));
}

TEST(join_multiple_channels)
{
	CommandRegistry	reg;
	FakeServer		srv;
	TestClient		tc;

	reg.registerCmds();
	registerClient(srv, tc, "alice");
	reg.execute(srv, irc::string2Message("JOIN #alpha,#beta", &tc.client));
	CHECK(srv.getChannelByTitle("#alpha") != 0);
	CHECK(srv.getChannelByTitle("#beta") != 0);
	CHECK(srv.getChannelByTitle("#alpha")->isChanOp(&tc.client));
	CHECK(srv.getChannelByTitle("#beta")->isChanOp(&tc.client));
}

TEST(join_wrong_key)
{
	CommandRegistry	reg;
	FakeServer		srv;
	TestClient		alice;
	TestClient		bob;

	reg.registerCmds();
	registerClient(srv, alice, "alice");
	registerClient(srv, bob, "bob");
	reg.execute(srv, irc::string2Message("JOIN #secret secret", &alice.client));
	srv.sent.clear();
	reg.execute(srv, irc::string2Message("JOIN #secret wrong", &bob.client));
	CHECK_EQ(lastTo(srv, &bob.client),
		std::string(":CoolServ 475 bob #secret :Cannot join channel (+k).\r\n"));
}

TEST(join_right_key_and_x_on_unkeyed)
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
	reg.execute(srv, irc::string2Message("JOIN #secret secret", &alice.client));
	srv.sent.clear();
	reg.execute(srv, irc::string2Message("JOIN #secret secret", &bob.client));
	CHECK(srv.getChannelByTitle("#secret")->isMember(&bob.client));
	srv.sent.clear();
	reg.execute(srv, irc::string2Message("JOIN #open", &alice.client));
	srv.sent.clear();
	reg.execute(srv, irc::string2Message("JOIN #open x", &carol.client));
	CHECK(srv.getChannelByTitle("#open")->isMember(&carol.client));
}

TEST(join_channel_full)
{
	CommandRegistry	reg;
	FakeServer		srv;
	TestClient		alice;
	TestClient		bob;
	Channel			*ch;

	reg.registerCmds();
	registerClient(srv, alice, "alice");
	registerClient(srv, bob, "bob");
	reg.execute(srv, irc::string2Message("JOIN #full", &alice.client));
	ch = srv.getChannelByTitle("#full");
	ch->setLimit(1);
	srv.sent.clear();
	reg.execute(srv, irc::string2Message("JOIN #full", &bob.client));
	CHECK_EQ(lastTo(srv, &bob.client),
		std::string(":CoolServ 471 bob #full :Cannot join channel (+l).\r\n"));
}

TEST(join_too_many_channels)
{
	CommandRegistry	reg;
	FakeServer		srv;
	TestClient		tc;

	reg.registerCmds();
	registerClient(srv, tc, "alice");
	reg.execute(srv, irc::string2Message("JOIN #one", &tc.client));
	reg.execute(srv, irc::string2Message("JOIN #two", &tc.client));
	reg.execute(srv, irc::string2Message("JOIN #tre", &tc.client));
	srv.sent.clear();
	reg.execute(srv, irc::string2Message("JOIN #four", &tc.client));
	CHECK_EQ(lastTo(srv, &tc.client),
		std::string(":CoolServ 405 alice #four :You have joined too many channels.\r\n"));
}

TEST(join_empty_topic_notopic)
{
	CommandRegistry	reg;
	FakeServer		srv;
	TestClient		alice;
	TestClient		bob;
	Channel			*ch;

	reg.registerCmds();
	registerClient(srv, alice, "alice");
	registerClient(srv, bob, "bob");
	reg.execute(srv, irc::string2Message("JOIN #empty", &alice.client));
	ch = srv.getChannelByTitle("#empty");
	ch->setTopic("");
	srv.sent.clear();
	reg.execute(srv, irc::string2Message("JOIN #empty", &bob.client));
	CHECK(sentContains(srv, &bob.client, " 331 "));
	CHECK(!sentContains(srv, &bob.client, " 332 "));
}

TEST(join_need_more_params)
{
	CommandRegistry	reg;
	FakeServer		srv;
	TestClient		tc;

	reg.registerCmds();
	registerClient(srv, tc, "alice");
	reg.execute(srv, irc::string2Message("JOIN", &tc.client));
	CHECK_EQ(lastTo(srv, &tc.client),
		std::string(":CoolServ 461 alice JOIN :Not enough parameters.\r\n"));
}

TEST(join_second_is_not_op)
{
	CommandRegistry	reg;
	FakeServer		srv;
	TestClient		alice;
	TestClient		bob;

	reg.registerCmds();
	registerClient(srv, alice, "alice");
	registerClient(srv, bob, "bob");
	reg.execute(srv, irc::string2Message("JOIN #chan", &alice.client));
	reg.execute(srv, irc::string2Message("JOIN #chan", &bob.client));
	CHECK(srv.getChannelByTitle("#chan")->isChanOp(&alice.client));
	CHECK(!srv.getChannelByTitle("#chan")->isChanOp(&bob.client));
}

TEST(join_case_folds_existing_channel)
{
	CommandRegistry	reg;
	FakeServer		srv;
	TestClient		alice;
	TestClient		bob;

	reg.registerCmds();
	registerClient(srv, alice, "alice");
	registerClient(srv, bob, "bob");
	reg.execute(srv, irc::string2Message("JOIN #Chan", &alice.client));
	reg.execute(srv, irc::string2Message("JOIN #chan", &bob.client));
	CHECK_EQ(srv.channels.size(), 1u);
	CHECK(srv.getChannelByTitle("#CHAN")->isMember(&bob.client));
}

TEST(join_ampersand)
{
	CommandRegistry	reg;
	FakeServer		srv;
	TestClient		tc;

	reg.registerCmds();
	registerClient(srv, tc, "alice");
	reg.execute(srv, irc::string2Message("JOIN &abc", &tc.client));
	CHECK(srv.getChannelByTitle("&abc") != 0);
	CHECK(srv.getChannelByTitle("&abc")->isMember(&tc.client));
}

TEST(join_invalid_charset)
{
	CommandRegistry	reg;
	FakeServer		srv;
	TestClient		tc;

	reg.registerCmds();
	registerClient(srv, tc, "alice");
	reg.execute(srv, irc::string2Message("JOIN #ch@n", &tc.client));
	CHECK_EQ(lastTo(srv, &tc.client),
		std::string(":CoolServ 476 alice #ch@n :Bad Channel Mask.\r\n"));
}

TEST(join_too_many_in_one_command)
{
	CommandRegistry	reg;
	FakeServer		srv;
	TestClient		tc;

	reg.registerCmds();
	registerClient(srv, tc, "alice");
	reg.execute(srv, irc::string2Message("JOIN #one,#two,#tre,#four",
			&tc.client));
	CHECK(srv.getChannelByTitle("#one") != 0);
	CHECK(srv.getChannelByTitle("#two") != 0);
	CHECK(srv.getChannelByTitle("#tre") != 0);
	CHECK(srv.getChannelByTitle("#four") == 0);
	CHECK_EQ(lastTo(srv, &tc.client),
		std::string(":CoolServ 405 alice #four :You have joined too many channels.\r\n"));
}

TEST(join_keys_paired_by_index)
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
	reg.execute(srv, irc::string2Message("JOIN #one,#two secret",
			&alice.client));
	srv.sent.clear();
	reg.execute(srv, irc::string2Message("JOIN #one,#two secret,x",
			&bob.client));
	CHECK(srv.getChannelByTitle("#one")->isMember(&bob.client));
	CHECK(srv.getChannelByTitle("#two")->isMember(&bob.client));
	srv.sent.clear();
	reg.execute(srv, irc::string2Message("JOIN #one,#two wrong,x",
			&carol.client));
	CHECK(!srv.getChannelByTitle("#one")->isMember(&carol.client));
	CHECK(srv.getChannelByTitle("#two")->isMember(&carol.client));
	CHECK(sentContains(srv, &carol.client,
		":CoolServ 475 carol #one :Cannot join channel (+k).\r\n"));
}

TEST(join_zero_is_bad_mask)
{
	CommandRegistry	reg;
	FakeServer		srv;
	TestClient		tc;

	reg.registerCmds();
	registerClient(srv, tc, "alice");
	reg.execute(srv, irc::string2Message("JOIN 0", &tc.client));
	CHECK_EQ(lastTo(srv, &tc.client),
		std::string(":CoolServ 476 alice 0 :Bad Channel Mask.\r\n"));
}

TEST(join_password_set_later)
{
	CommandRegistry	reg;
	FakeServer		srv;
	TestClient		alice;
	TestClient		bob;
	Channel			*ch;

	reg.registerCmds();
	registerClient(srv, alice, "alice");
	registerClient(srv, bob, "bob");
	reg.execute(srv, irc::string2Message("JOIN #later", &alice.client));
	ch = srv.getChannelByTitle("#later");
	ch->setPassword("secret");
	srv.sent.clear();
	reg.execute(srv, irc::string2Message("JOIN #later", &bob.client));
	CHECK_EQ(lastTo(srv, &bob.client),
		std::string(":CoolServ 475 bob #later :Cannot join channel (+k).\r\n"));
}

TEST(join_invite_only_without_invite)
{
	CommandRegistry	reg;
	FakeServer		srv;
	TestClient		alice;
	TestClient		bob;

	reg.registerCmds();
	registerClient(srv, alice, "alice");
	registerClient(srv, bob, "bob");
	reg.execute(srv, irc::string2Message("JOIN #chan", &alice.client));
	reg.execute(srv, irc::string2Message("MODE #chan +i", &alice.client));
	srv.sent.clear();
	reg.execute(srv, irc::string2Message("JOIN #chan", &bob.client));
	CHECK_EQ(lastTo(srv, &bob.client),
		std::string(":CoolServ 473 bob #chan :Cannot join channel (+i).\r\n"));
	CHECK(!srv.getChannelByTitle("#chan")->isMember(&bob.client));
}

TEST(join_after_invite_on_invite_only)
{
	CommandRegistry	reg;
	FakeServer		srv;
	TestClient		alice;
	TestClient		bob;

	reg.registerCmds();
	registerClient(srv, alice, "alice");
	registerClient(srv, bob, "bob");
	reg.execute(srv, irc::string2Message("JOIN #chan", &alice.client));
	reg.execute(srv, irc::string2Message("MODE #chan +i", &alice.client));
	reg.execute(srv, irc::string2Message("INVITE bob #chan", &alice.client));
	srv.sent.clear();
	reg.execute(srv, irc::string2Message("JOIN #chan", &bob.client));
	CHECK(srv.getChannelByTitle("#chan")->isMember(&bob.client));
}
