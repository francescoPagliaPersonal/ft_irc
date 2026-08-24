/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_commands.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mweghofe <mweghofe@student.42vienna.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/24 10:48:00 by mweghofe          #+#    #+#             */
/*   Updated: 2026/08/24 10:48:00 by mweghofe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "CommandRegistry.hpp"
#include "Message.hpp"
#include "client_fixture.hpp"
#include "fake_server.hpp"
#include "ft_irc.hpp"
#include "harness.hpp"

#include <string>

namespace
{
	int	runCmd(CommandRegistry &reg, FakeServer &srv, Client &c,
			const std::string &line)
	{
		Message	msg = string2Message(line, &c);

		return (reg.execute(srv, msg));
	}

	std::string	lastTo(const FakeServer &srv, Client *c)
	{
		if (srv.sent.empty())
			return (std::string());
		for (size_t i = srv.sent.size(); i > 0; --i)
		{
			if (srv.sent[i - 1].first == c)
				return (srv.sent[i - 1].second);
		}
		return (std::string());
	}
}

TEST(pass_ok)
{
	CommandRegistry	reg;
	FakeServer		srv;
	TestClient		tc;

	reg.registerCmds();
	srv.password = "1o.0";
	CHECK_EQ(runCmd(reg, srv, tc.client, "PASS 1o.0"), rfc::OK);
	CHECK(tc.client.getRegistrationFlags() & REG_PASSWD);
}

TEST(pass_mismatch)
{
	CommandRegistry	reg;
	FakeServer		srv;
	TestClient		tc;

	reg.registerCmds();
	srv.password = "1o.0";
	CHECK_EQ(runCmd(reg, srv, tc.client, "PASS wrong"), rfc::BADPASS);
	CHECK(!(tc.client.getRegistrationFlags() & REG_PASSWD));
}

TEST(pass_second_is_already_registered)
{
	CommandRegistry	reg;
	FakeServer		srv;
	TestClient		tc;

	reg.registerCmds();
	srv.password = "1o.0";
	CHECK_EQ(runCmd(reg, srv, tc.client, "PASS 1o.0"), rfc::OK);
	CHECK_EQ(runCmd(reg, srv, tc.client, "PASS 1o.0"), rfc::ALREADYREG);
}

TEST(nick_missing_param)
{
	CommandRegistry	reg;
	FakeServer		srv;
	TestClient		tc;

	reg.registerCmds();
	CHECK_EQ(runCmd(reg, srv, tc.client, "NICK"), rfc::FEWPARAMS);
}

TEST(nick_bad)
{
	CommandRegistry	reg;
	FakeServer		srv;
	TestClient		tc;

	reg.registerCmds();
	CHECK_EQ(runCmd(reg, srv, tc.client, "NICK #bad"), rfc::NICKBAD);
	CHECK_EQ(runCmd(reg, srv, tc.client, "NICK a.b"), rfc::NICKBAD);
	CHECK_EQ(runCmd(reg, srv, tc.client,
		"NICK aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"), rfc::NICKBAD);
}

TEST(nick_collision)
{
	CommandRegistry	reg;
	FakeServer		srv;
	TestClient		a;
	TestClient		b;

	reg.registerCmds();
	b.client.setNick("alice");
	srv.nicks["alice"] = &b.client;
	CHECK_EQ(runCmd(reg, srv, a.client, "NICK alice"), rfc::NICKINUSE);
	CHECK_EQ(a.client.getNick(), std::string("*"));
}

TEST(nick_success_sets_flag)
{
	CommandRegistry	reg;
	FakeServer		srv;
	TestClient		tc;

	reg.registerCmds();
	CHECK_EQ(runCmd(reg, srv, tc.client, "NICK alice"), rfc::OK);
	CHECK_EQ(tc.client.getNick(), std::string("alice"));
	CHECK(tc.client.getRegistrationFlags() & REG_NICK);
}

TEST(user_sets_names)
{
	CommandRegistry	reg;
	FakeServer		srv;
	TestClient		tc;

	reg.registerCmds();
	CHECK_EQ(runCmd(reg, srv, tc.client, "USER ident 0 * :real name"),
		rfc::OK);
	CHECK_EQ(tc.client.getUserName(), std::string("ident"));
	CHECK_EQ(tc.client.getRealName(), std::string("real name"));
	CHECK(tc.client.getRegistrationFlags() & REG_USER);
}

TEST(user_twice_before_done)
{
	CommandRegistry	reg;
	FakeServer		srv;
	TestClient		tc;

	reg.registerCmds();
	CHECK_EQ(runCmd(reg, srv, tc.client, "USER ident 0 * :real"), rfc::OK);
	CHECK_EQ(runCmd(reg, srv, tc.client, "USER other 0 * :name"),
		rfc::ALREADYREG);
}

TEST(cap_ls_sets_cap_and_replies)
{
	CommandRegistry	reg;
	FakeServer		srv;
	TestClient		tc;

	reg.registerCmds();
	CHECK_EQ(runCmd(reg, srv, tc.client, "CAP LS"), rfc::OK);
	CHECK(tc.client.getCap());
	CHECK_EQ(lastTo(srv, &tc.client),
		std::string(":CoolServ CAP * LS :\r\n"));
}

TEST(cap_end_without_ls_is_invalid)
{
	CommandRegistry	reg;
	FakeServer		srv;
	TestClient		tc;

	reg.registerCmds();
	CHECK_EQ(runCmd(reg, srv, tc.client, "CAP END"), rfc::INVALIDCAPCMD);
}

TEST(cap_unknown_subcmd)
{
	CommandRegistry	reg;
	FakeServer		srv;
	TestClient		tc;

	reg.registerCmds();
	CHECK_EQ(runCmd(reg, srv, tc.client, "CAP REQ"), rfc::INVALIDCAPCMD);
}

TEST(ping_token_from_param)
{
	CommandRegistry	reg;
	FakeServer		srv;
	TestClient		tc;

	reg.registerCmds();
	CHECK_EQ(runCmd(reg, srv, tc.client, "PING 12345"), rfc::OK);
	CHECK_EQ(lastTo(srv, &tc.client),
		std::string(":CoolServ PONG CoolServ :12345\r\n"));
}

TEST(ping_token_from_trailing)
{
	CommandRegistry	reg;
	FakeServer		srv;
	TestClient		tc;

	reg.registerCmds();
	CHECK_EQ(runCmd(reg, srv, tc.client, "PING :lag"), rfc::OK);
	CHECK_EQ(lastTo(srv, &tc.client),
		std::string(":CoolServ PONG CoolServ :lag\r\n"));
}

TEST(privmsg_requires_registration)
{
	CommandRegistry	reg;
	FakeServer		srv;
	TestClient		tc;

	reg.registerCmds();
	CHECK_EQ(runCmd(reg, srv, tc.client, "PRIVMSG bob :hello"), rfc::NOTREG);
}

TEST(privmsg_no_text)
{
	CommandRegistry	reg;
	FakeServer		srv;
	TestClient		tc;

	reg.registerCmds();
	tc.client.setRegistrationFlags(REG_DONE);
	CHECK_EQ(runCmd(reg, srv, tc.client, "PRIVMSG bob extra"), rfc::NOTEXT);
}

TEST(privmsg_unknown_nick)
{
	CommandRegistry	reg;
	FakeServer		srv;
	TestClient		tc;

	reg.registerCmds();
	tc.client.setRegistrationFlags(REG_DONE);
	CHECK_EQ(runCmd(reg, srv, tc.client, "PRIVMSG bob :hello"),
		rfc::NOSUCHNICK);
}

TEST(privmsg_success)
{
	CommandRegistry	reg;
	FakeServer		srv;
	TestClient		sender;
	TestClient		dest;

	reg.registerCmds();
	sender.client.setNick("alice");
	sender.client.setUserName("user");
	sender.client.setRegistrationFlags(REG_DONE);
	dest.client.setNick("bob");
	srv.nicks["bob"] = &dest.client;
	CHECK_EQ(runCmd(reg, srv, sender.client, "PRIVMSG bob :hello"), rfc::OK);
	CHECK_EQ(srv.sent.size(), 1u);
	CHECK_EQ(srv.sent[0].first, &dest.client);
	CHECK_EQ(srv.sent[0].second,
		std::string(":alice!user PRIVMSG bob :hello\r\n"));
}

TEST(unknown_command)
{
	CommandRegistry	reg;
	FakeServer		srv;
	TestClient		tc;

	reg.registerCmds();
	CHECK_EQ(runCmd(reg, srv, tc.client, "NOSUCH"), rfc::BADCMD);
}

TEST(proto_badcmd_text)
{
	CommandRegistry	reg;
	FakeServer		srv;
	TestClient		tc;
	Message			msg = string2Message("NOSUCH", &tc.client);

	CHECK(reg.handleProtocolErrors(srv, rfc::BADCMD, msg));
	CHECK_EQ(srv.sent[0].second,
		std::string(":CoolServer 421 NOSUCH :Command not found.\r\n"));
}

TEST(proto_nonick_text)
{
	CommandRegistry	reg;
	FakeServer		srv;
	TestClient		tc;
	Message			msg = string2Message("NICK", &tc.client);

	CHECK(reg.handleProtocolErrors(srv, rfc::NONICK, msg));
	CHECK_EQ(srv.sent[0].second,
		std::string(":CoolServer 431  :No nickname given.\r\n"));
}

TEST(proto_nickinuse_text)
{
	CommandRegistry	reg;
	FakeServer		srv;
	TestClient		tc;
	Message			msg = string2Message("NICK alice", &tc.client);

	CHECK(reg.handleProtocolErrors(srv, rfc::NICKINUSE, msg));
	CHECK_EQ(srv.sent[0].second,
		std::string(":CoolServer 433 * alice :Nickname is already in use.\r\n"));
}

TEST(proto_nickbad_text)
{
	CommandRegistry	reg;
	FakeServer		srv;
	TestClient		tc;
	Message			msg = string2Message("NICK #x", &tc.client);

	CHECK(reg.handleProtocolErrors(srv, rfc::NICKBAD, msg));
	CHECK_EQ(srv.sent[0].second,
		std::string(":CoolServer 432 * #x :Erroneous Nickname.\r\n"));
}

TEST(proto_notreg_text)
{
	CommandRegistry	reg;
	FakeServer		srv;
	TestClient		tc;
	Message			msg = string2Message("PRIVMSG x :y", &tc.client);

	CHECK(reg.handleProtocolErrors(srv, rfc::NOTREG, msg));
	CHECK_EQ(srv.sent[0].second,
		std::string(":CoolServ 451 <nick> :You have not registered.\r\n"));
}

TEST(proto_fewparams_text)
{
	CommandRegistry	reg;
	FakeServer		srv;
	TestClient		tc;
	Message			msg = string2Message("PASS", &tc.client);

	CHECK(reg.handleProtocolErrors(srv, rfc::FEWPARAMS, msg));
	CHECK_EQ(srv.sent[0].second,
		std::string(":CoolServer 461 * PASS :Not enough parameters.\r\n"));
}

TEST(proto_alreadyreg_text)
{
	CommandRegistry	reg;
	FakeServer		srv;
	TestClient		tc;
	Message			msg = string2Message("PASS x", &tc.client);

	CHECK(reg.handleProtocolErrors(srv, rfc::ALREADYREG, msg));
	CHECK_EQ(srv.sent[0].second,
		std::string(":CoolServ 462 <nick> :This user is already registered.\r\n"));
}

TEST(proto_badpass_text)
{
	CommandRegistry	reg;
	FakeServer		srv;
	TestClient		tc;
	Message			msg = string2Message("PASS x", &tc.client);

	CHECK(reg.handleProtocolErrors(srv, rfc::BADPASS, msg));
	CHECK_EQ(srv.sent[0].second,
		std::string(":CoolServ 464 <nick> :Password incorrect.\r\n"));
}

TEST(proto_noconn_disconnects)
{
	CommandRegistry	reg;
	FakeServer		srv;
	TestClient		tc;
	Message			msg = string2Message("PING x", &tc.client);

	CHECK(!reg.handleProtocolErrors(srv, rfc::NOCONN, msg));
	CHECK(srv.sent.empty());
}

TEST(registration_needs_all_three)
{
	CommandRegistry	reg;
	FakeServer		srv;
	TestClient		tc;

	reg.registerCmds();
	srv.password = "1o.0";
	CHECK_EQ(runCmd(reg, srv, tc.client, "PASS 1o.0"), rfc::OK);
	CHECK_EQ(runCmd(reg, srv, tc.client, "NICK alice"), rfc::OK);
	CHECK_EQ(srv.completeCalls, 0);
	CHECK_EQ(runCmd(reg, srv, tc.client, "USER u 0 * :Real Name"), rfc::OK);
	CHECK_EQ(tc.client.getRegistrationFlags(), static_cast<int>(REG_DONE));
	CHECK_EQ(srv.completeCalls, 1);
}

TEST(registration_delayed_by_cap)
{
	CommandRegistry	reg;
	FakeServer		srv;
	TestClient		tc;

	reg.registerCmds();
	srv.password = "1o.0";
	CHECK_EQ(runCmd(reg, srv, tc.client, "CAP LS"), rfc::OK);
	CHECK(tc.client.getCap());
	CHECK_EQ(runCmd(reg, srv, tc.client, "PASS 1o.0"), rfc::OK);
	CHECK_EQ(runCmd(reg, srv, tc.client, "NICK alice"), rfc::OK);
	CHECK_EQ(runCmd(reg, srv, tc.client, "USER u 0 * :Real"), rfc::OK);
	CHECK_EQ(srv.completeCalls, 0);
	CHECK_EQ(runCmd(reg, srv, tc.client, "CAP END"), rfc::OK);
	CHECK_EQ(srv.completeCalls, 1);
	CHECK(!tc.client.getCap());
}
