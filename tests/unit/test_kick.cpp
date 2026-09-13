/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_kick.cpp                                      :+:      :+:    :+:   */
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

TEST(kick_by_op_removes_member)
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
	reg.execute(srv, irc::string2Message("KICK #chan bob :out", &alice.client));
	CHECK(!srv.getChannelByTitle("#chan")->isMember(&bob.client));
	CHECK(sentContains(srv, &bob.client, " KICK #chan bob"));
}

TEST(kick_non_op)
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
	reg.execute(srv, irc::string2Message("KICK #chan alice :nope", &bob.client));
	CHECK_EQ(lastTo(srv, &bob.client),
		std::string(":CoolServ 482 bob #chan :You're not channel operator.\r\n"));
	CHECK(srv.getChannelByTitle("#chan")->isMember(&alice.client));
}

TEST(kick_user_not_in_channel)
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
	reg.execute(srv, irc::string2Message("KICK #chan bob :x", &alice.client));
	CHECK_EQ(lastTo(srv, &alice.client),
		std::string(":CoolServ 441 alice bob :They aren't on that channel.\r\n"));
}

TEST(kick_not_on_channel)
{
	CommandRegistry	reg;
	FakeServer		srv;
	TestClient		alice;
	TestClient		bob;

	reg.registerCmds();
	registerClient(srv, alice, "alice");
	registerClient(srv, bob, "bob");
	joinChannel(reg, srv, bob, "#chan");
	srv.sent.clear();
	reg.execute(srv, irc::string2Message("KICK #chan nobody :x", &alice.client));
	CHECK_EQ(lastTo(srv, &alice.client),
		std::string(":CoolServ 442 alice #chan :You're not on that channel.\r\n"));
}

TEST(kick_no_such_channel)
{
	CommandRegistry	reg;
	FakeServer		srv;
	TestClient		alice;

	reg.registerCmds();
	registerClient(srv, alice, "alice");
	reg.execute(srv, irc::string2Message("KICK #nope bob :x", &alice.client));
	CHECK_EQ(lastTo(srv, &alice.client),
		std::string(":CoolServ 403 alice #nope :No such channel.\r\n"));
}

TEST(kick_default_message_without_trailing)
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
	reg.execute(srv, irc::string2Message("KICK #chan bob", &alice.client));
	CHECK(sentContains(srv, &bob.client,
		" KICK #chan bob :must have done something wrong."));
}

TEST(kick_multi_user_broadcast)
{
	CommandRegistry	reg;
	FakeServer		srv;
	TestClient		alice;
	TestClient		bob;
	TestClient		carol;
	TestClient		dave;

	reg.registerCmds();
	registerClient(srv, alice, "alice");
	registerClient(srv, bob, "bob");
	registerClient(srv, carol, "carol");
	registerClient(srv, dave, "dave");
	joinChannel(reg, srv, alice, "#chan");
	joinChannel(reg, srv, bob, "#chan");
	joinChannel(reg, srv, carol, "#chan");
	joinChannel(reg, srv, dave, "#chan");
	srv.sent.clear();
	reg.execute(srv, irc::string2Message("KICK #chan bob,carol :out",
			&alice.client));
	CHECK(!srv.getChannelByTitle("#chan")->isMember(&bob.client));
	CHECK(!srv.getChannelByTitle("#chan")->isMember(&carol.client));
	CHECK(sentContains(srv, &dave.client, " KICK #chan bob"));
	CHECK(sentContains(srv, &dave.client, " KICK #chan carol"));
}

TEST(kick_empty_channel_deleted)
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
	reg.execute(srv, irc::string2Message("KICK #chan bob,alice", &alice.client));
	CHECK(srv.getChannelByTitle("#chan") == 0);
}

TEST(kick_rejoin_after_empty_is_founder_op)
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
	reg.execute(srv, irc::string2Message("KICK #chan bob,alice", &alice.client));
	srv.sent.clear();
	joinChannel(reg, srv, bob, "#chan");
	Channel	*ch = srv.getChannelByTitle("#chan");

	CHECK(ch != 0);
	CHECK(ch->isChanOp(&bob.client));
	CHECK(sentContains(srv, &bob.client, "@bob"));
}
