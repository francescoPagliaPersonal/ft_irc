/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_invite.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mweghofe <mweghofe@student.42vienna.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/31 10:33:00 by mweghofe          #+#    #+#             */
/*   Updated: 2026/08/31 10:33:00 by mweghofe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "CommandRegistry.hpp"
#include "Message.hpp"
#include "client_fixture.hpp"
#include "fake_server.hpp"
#include "harness.hpp"
#include "test_cmd_helper.hpp"

#include <string>

TEST(invite_no_such_channel)
{
	CommandRegistry	reg;
	FakeServer		srv;
	TestClient		alice;
	TestClient		bob;

	reg.registerCmds();
	registerClient(srv, alice, "alice");
	registerClient(srv, bob, "bob");
	reg.execute(srv, irc::string2Message("INVITE bob #nope", &alice.client));
	CHECK_EQ(lastTo(srv, &alice.client),
		std::string(":CoolServ 403 alice bob :No such channel.\r\n"));
}

TEST(invite_no_such_nick)
{
	CommandRegistry	reg;
	FakeServer		srv;
	TestClient		alice;

	reg.registerCmds();
	registerClient(srv, alice, "alice");
	reg.execute(srv, irc::string2Message("JOIN #chan", &alice.client));
	srv.sent.clear();
	reg.execute(srv, irc::string2Message("INVITE ghost #chan", &alice.client));
	CHECK_EQ(lastTo(srv, &alice.client),
		std::string(":CoolServ 401 alice ghost :No such nick.\r\n"));
}

TEST(invite_user_already_on_channel)
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
	srv.sent.clear();
	reg.execute(srv, irc::string2Message("INVITE bob #chan", &alice.client));
	CHECK_EQ(lastTo(srv, &alice.client),
		std::string(":CoolServ 443 alice bob :is already on channel.\r\n"));
}

TEST(invite_inviter_not_on_channel)
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
	reg.execute(srv, irc::string2Message("JOIN #chan", &bob.client));
	srv.sent.clear();
	reg.execute(srv, irc::string2Message("INVITE carol #chan", &alice.client));
	CHECK_EQ(lastTo(srv, &alice.client),
		std::string(":CoolServ 442 alice :You're not on that channel.\r\n"));
}

TEST(invite_success)
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
	reg.execute(srv, irc::string2Message("INVITE bob #chan", &alice.client));
	CHECK(sentContains(srv, &bob.client, " INVITE bob #chan"));
	CHECK_EQ(lastTo(srv, &alice.client),
		std::string(":CoolServ 341 alice bob #chan\r\n"));
}

/* needs MODE
TEST(invite_non_op_on_invite_only)
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
	reg.execute(srv, irc::string2Message("JOIN #chan", &alice.client));
	reg.execute(srv, irc::string2Message("MODE #chan +i", &alice.client));
	reg.execute(srv, irc::string2Message("JOIN #chan", &bob.client));
	srv.sent.clear();
	reg.execute(srv, irc::string2Message("INVITE carol #chan", &bob.client));
	CHECK_EQ(lastTo(srv, &bob.client),
		std::string(":CoolServ 482 bob :You're not channel operator.\r\n"));
}

TEST(invite_then_join_on_invite_only)
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
*/
