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
