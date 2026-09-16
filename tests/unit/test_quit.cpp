/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_quit.cpp                                      :+:      :+:    :+:   */
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

TEST(quit_requires_registration)
{
	CommandRegistry	reg;
	FakeServer		srv;
	TestClient		tc;

	reg.registerCmds();
	reg.execute(srv, irc::string2Message("QUIT :bye", &tc.client));
	CHECK_EQ(lastTo(srv, &tc.client),
		std::string(":CoolServ 451 * :You have not registered\r\n"));
	CHECK(!tc.client.hasQuit());
}

TEST(quit_sets_has_quit_and_error)
{
	CommandRegistry	reg;
	FakeServer		srv;
	TestClient		alice;

	reg.registerCmds();
	registerClient(srv, alice, "alice");
	reg.execute(srv, irc::string2Message("QUIT :bye", &alice.client));
	CHECK(alice.client.hasQuit());
	CHECK(sentContains(srv, &alice.client, "ERROR"));
	CHECK(sentContains(srv, &alice.client, "Closing link"));
}

TEST(quit_broadcasts_to_channel_peer)
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
	reg.execute(srv, irc::string2Message("QUIT :gone", &alice.client));
	CHECK(sentContains(srv, &bob.client, " QUIT "));
	CHECK(sentContains(srv, &bob.client, ":alice!user@0.0.0.0 QUIT"));
	CHECK(!srv.getChannelByTitle("#chan")->isMember(&alice.client));
}

TEST(quit_without_trailing)
{
	CommandRegistry	reg;
	FakeServer		srv;
	TestClient		alice;

	reg.registerCmds();
	registerClient(srv, alice, "alice");
	reg.execute(srv, irc::string2Message("QUIT", &alice.client));
	CHECK(alice.client.hasQuit());
	CHECK(sentContains(srv, &alice.client, "ERROR"));
}
