/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_topic.cpp                                     :+:      :+:    :+:   */
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

TEST(topic_no_such_channel)
{
	CommandRegistry	reg;
	FakeServer		srv;
	TestClient		alice;

	reg.registerCmds();
	registerClient(srv, alice, "alice");
	reg.execute(srv, irc::string2Message("TOPIC #nope", &alice.client));
	CHECK_EQ(lastTo(srv, &alice.client),
		std::string(":CoolServ 403 alice #nope :No such channel.\r\n"));
}

TEST(topic_not_on_channel)
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
	reg.execute(srv, irc::string2Message("TOPIC #chan", &bob.client));
	CHECK_EQ(lastTo(srv, &bob.client),
		std::string(":CoolServ 442 bob #chan :You're not on that channel.\r\n"));
}

TEST(topic_view_empty)
{
	CommandRegistry	reg;
	FakeServer		srv;
	TestClient		alice;

	reg.registerCmds();
	registerClient(srv, alice, "alice");
	joinChannel(reg, srv, alice, "#chan");
	srv.sent.clear();
	reg.execute(srv, irc::string2Message("TOPIC #chan", &alice.client));
	CHECK_EQ(lastTo(srv, &alice.client),
		std::string(":CoolServ 331 alice :No topic is set\r\n"));
}

TEST(topic_set_and_view)
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
	reg.execute(srv, irc::string2Message("TOPIC #chan :hello topic",
			&alice.client));
	CHECK_EQ(srv.getChannelByTitle("#chan")->getTopic(),
		std::string("hello topic"));
	CHECK(sentContains(srv, &alice.client, " 332 "));
	CHECK(sentContains(srv, &bob.client, " 332 "));
	CHECK(sentContains(srv, &bob.client, "hello topic"));
}

TEST(topic_non_op_with_plus_t)
{
	CommandRegistry	reg;
	FakeServer		srv;
	TestClient		alice;
	TestClient		bob;

	reg.registerCmds();
	registerClient(srv, alice, "alice");
	registerClient(srv, bob, "bob");
	joinChannel(reg, srv, alice, "#chan");
	reg.execute(srv, irc::string2Message("MODE #chan +t", &alice.client));
	joinChannel(reg, srv, bob, "#chan");
	srv.sent.clear();
	reg.execute(srv, irc::string2Message("TOPIC #chan :hijack", &bob.client));
	CHECK_EQ(lastTo(srv, &bob.client),
		std::string(":CoolServ 482 bob #chan :You're not channel operator.\r\n"));
}
