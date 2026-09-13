/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_part.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mweghofe <mweghofe@student.42vienna.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/13 00:00:00 by mweghofe          #+#    #+#             */
/*   Updated: 2026/09/13 00:00:00 by mweghofe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "CommandRegistry.hpp"
#include "Message.hpp"
#include "client_fixture.hpp"
#include "fake_server.hpp"
#include "harness.hpp"
#include "test_cmd_helper.hpp"

#include <string>

TEST(part_member_leaves_and_peer_sees)
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
	reg.execute(srv, irc::string2Message("PART #chan", &bob.client));
	CHECK(!srv.getChannelByTitle("#chan")->isMember(&bob.client));
	CHECK(sentContains(srv, &alice.client, " PART #chan"));
}

TEST(part_last_member_deletes_channel)
{
	CommandRegistry	reg;
	FakeServer		srv;
	TestClient		alice;

	reg.registerCmds();
	registerClient(srv, alice, "alice");
	joinChannel(reg, srv, alice, "#chan");
	srv.sent.clear();
	reg.execute(srv, irc::string2Message("PART #chan", &alice.client));
	CHECK(srv.getChannelByTitle("#chan") == 0);
}

TEST(part_no_such_channel)
{
	CommandRegistry	reg;
	FakeServer		srv;
	TestClient		alice;

	reg.registerCmds();
	registerClient(srv, alice, "alice");
	reg.execute(srv, irc::string2Message("PART #nope", &alice.client));
	CHECK_EQ(lastTo(srv, &alice.client),
		std::string(":CoolServ 403 alice #nope :No such channel.\r\n"));
}

TEST(part_not_on_channel)
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
	reg.execute(srv, irc::string2Message("PART #chan", &bob.client));
	CHECK_EQ(lastTo(srv, &bob.client),
		std::string(":CoolServ 442 bob #chan :You're not on that channel.\r\n"));
}

TEST(part_with_reason)
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
	reg.execute(srv, irc::string2Message("PART #chan :goodbye", &bob.client));
	CHECK(sentContains(srv, &alice.client, " PART #chan :goodbye"));
}

TEST(part_comma_list)
{
	CommandRegistry	reg;
	FakeServer		srv;
	TestClient		alice;

	reg.registerCmds();
	registerClient(srv, alice, "alice");
	joinChannel(reg, srv, alice, "#one");
	joinChannel(reg, srv, alice, "#two");
	srv.sent.clear();
	reg.execute(srv, irc::string2Message("PART #one,#two", &alice.client));
	CHECK(srv.getChannelByTitle("#one") == 0);
	CHECK(srv.getChannelByTitle("#two") == 0);
}
