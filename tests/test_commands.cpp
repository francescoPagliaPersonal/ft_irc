/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_commands.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mweghofe <mweghofe@student.42vienna.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/24 10:48:00 by mweghofe          #+#    #+#             */
/*   Updated: 2026/08/31 09:40:00 by mweghofe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "CommandRegistry.hpp"
#include "Message.hpp"
#include "Response.hpp"
#include "client_fixture.hpp"
#include "fake_server.hpp"
#include "irc.hpp"
#include "harness.hpp"

#include <string>

namespace
{
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
	reg.execute(srv, irc::string2Message("PASS 1o.0", &tc.client));
	CHECK(tc.client.getRegistrationFlags() & REG_PASSWD);
	CHECK(srv.sent.empty());
}

TEST(pass_mismatch)
{
	CommandRegistry	reg;
	FakeServer		srv;
	TestClient		tc;

	reg.registerCmds();
	srv.password = "1o.0";
	reg.execute(srv, irc::string2Message("PASS wrong", &tc.client));
	CHECK(!(tc.client.getRegistrationFlags() & REG_PASSWD));
	CHECK_EQ(lastTo(srv, &tc.client),
		std::string(":CoolServ 464 * :Password incorrect.\r\n"));
}

TEST(pass_second_is_already_registered)
{
	CommandRegistry	reg;
	FakeServer		srv;
	TestClient		tc;

	reg.registerCmds();
	srv.password = "1o.0";
	reg.execute(srv, irc::string2Message("PASS 1o.0", &tc.client));
	srv.sent.clear();
	reg.execute(srv, irc::string2Message("PASS 1o.0", &tc.client));
	CHECK_EQ(lastTo(srv, &tc.client),
		std::string(":CoolServ 462 * :You may not reregister.\r\n"));
}

TEST(nick_missing_param)
{
	CommandRegistry	reg;
	FakeServer		srv;
	TestClient		tc;

	reg.registerCmds();
	reg.execute(srv, irc::string2Message("NICK", &tc.client));
	CHECK_EQ(lastTo(srv, &tc.client),
		std::string(":CoolServ 461 * NICK :Not enough parameters.\r\n"));
}

TEST(nick_bad)
{
	CommandRegistry	reg;
	FakeServer		srv;
	TestClient		tc;

	reg.registerCmds();
	reg.execute(srv, irc::string2Message("NICK #bad", &tc.client));
	CHECK_EQ(lastTo(srv, &tc.client),
		std::string(":CoolServ 432 * #bad :Erroneus nickname.\r\n"));
	srv.sent.clear();
	reg.execute(srv, irc::string2Message("NICK a.b", &tc.client));
	CHECK_EQ(lastTo(srv, &tc.client),
		std::string(":CoolServ 432 * a.b :Erroneus nickname.\r\n"));
	srv.sent.clear();
	reg.execute(srv, irc::string2Message("NICK aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa", &tc.client));
	CHECK_EQ(lastTo(srv, &tc.client),
		std::string(":CoolServ 432 * aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa :Erroneus nickname.\r\n"));
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
	reg.execute(srv, irc::string2Message("NICK alice", &a.client));
	CHECK_EQ(lastTo(srv, &a.client),
		std::string(":CoolServ 433 * alice :Nickname is already in use.\r\n"));
	CHECK_EQ(a.client.getNick(), std::string("*"));
}

TEST(nick_success_sets_flag)
{
	CommandRegistry	reg;
	FakeServer		srv;
	TestClient		tc;

	reg.registerCmds();
	reg.execute(srv, irc::string2Message("NICK alice", &tc.client));
	CHECK_EQ(tc.client.getNick(), std::string("alice"));
	CHECK(tc.client.getRegistrationFlags() & REG_NICK);
	CHECK(srv.sent.empty());
}

TEST(user_sets_names)
{
	CommandRegistry	reg;
	FakeServer		srv;
	TestClient		tc;

	reg.registerCmds();
	reg.execute(srv, irc::string2Message("USER ident 0 * :real name", &tc.client));
	CHECK_EQ(tc.client.getUserName(), std::string("ident"));
	CHECK_EQ(tc.client.getRealName(), std::string("real name"));
	CHECK(tc.client.getRegistrationFlags() & REG_USER);
	CHECK(srv.sent.empty());
}

TEST(user_twice_before_done)
{
	CommandRegistry	reg;
	FakeServer		srv;
	TestClient		tc;

	reg.registerCmds();
	reg.execute(srv, irc::string2Message("USER ident 0 * :real", &tc.client));
	srv.sent.clear();
	reg.execute(srv, irc::string2Message("USER other 0 * :name", &tc.client));
	CHECK_EQ(lastTo(srv, &tc.client),
		std::string(":CoolServ 462 * :You may not reregister.\r\n"));
}

TEST(cap_ls_sets_cap_and_replies)
{
	CommandRegistry	reg;
	FakeServer		srv;
	TestClient		tc;

	reg.registerCmds();
	reg.execute(srv, irc::string2Message("CAP LS", &tc.client));
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
	reg.execute(srv, irc::string2Message("CAP END", &tc.client));
	CHECK_EQ(lastTo(srv, &tc.client),
		std::string(":CoolServ 410 * END No such CAP command.\r\n"));
}

TEST(cap_unknown_subcmd)
{
	CommandRegistry	reg;
	FakeServer		srv;
	TestClient		tc;

	reg.registerCmds();
	reg.execute(srv, irc::string2Message("CAP REQ", &tc.client));
	CHECK_EQ(lastTo(srv, &tc.client),
		std::string(":CoolServ 410 * REQ No such CAP command.\r\n"));
}

TEST(ping_token_from_param)
{
	CommandRegistry	reg;
	FakeServer		srv;
	TestClient		tc;

	reg.registerCmds();
	reg.execute(srv, irc::string2Message("PING 12345", &tc.client));
	CHECK_EQ(lastTo(srv, &tc.client),
		std::string(":CoolServ PONG CoolServ :12345\r\n"));
}

TEST(ping_token_from_trailing)
{
	CommandRegistry	reg;
	FakeServer		srv;
	TestClient		tc;

	reg.registerCmds();
	reg.execute(srv, irc::string2Message("PING :lag", &tc.client));
	CHECK_EQ(lastTo(srv, &tc.client),
		std::string(":CoolServ PONG CoolServ :lag\r\n"));
}

TEST(privmsg_requires_registration)
{
	CommandRegistry	reg;
	FakeServer		srv;
	TestClient		tc;

	reg.registerCmds();
	reg.execute(srv, irc::string2Message("PRIVMSG bob :hello", &tc.client));
	CHECK_EQ(lastTo(srv, &tc.client),
		std::string(":CoolServ 451 * :You have not registered\r\n"));
}

TEST(privmsg_no_text)
{
	CommandRegistry	reg;
	FakeServer		srv;
	TestClient		tc;

	reg.registerCmds();
	tc.client.setRegistrationFlags(REG_DONE);
	reg.execute(srv, irc::string2Message("PRIVMSG bob extra", &tc.client));
	CHECK_EQ(lastTo(srv, &tc.client),
		std::string(":CoolServ 412 * :No text to send.\r\n"));
}

TEST(privmsg_unknown_nick)
{
	CommandRegistry	reg;
	FakeServer		srv;
	TestClient		tc;

	reg.registerCmds();
	tc.client.setRegistrationFlags(REG_DONE);
	reg.execute(srv, irc::string2Message("PRIVMSG bob :hello", &tc.client));
	CHECK_EQ(lastTo(srv, &tc.client),
		std::string(":CoolServ 401 * :No such nick.\r\n"));
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
	reg.execute(srv, irc::string2Message("PRIVMSG bob :hello", &sender.client));
	CHECK_EQ(srv.sent.size(), 1u);
	CHECK_EQ(srv.sent[0].first, &dest.client);
	CHECK_EQ(srv.sent[0].second,
		std::string(":alice!user@0.0.0.0 PRIVMSG bob :hello\r\n"));
}

TEST(unknown_command)
{
	CommandRegistry	reg;
	FakeServer		srv;
	TestClient		tc;

	reg.registerCmds();
	reg.execute(srv, irc::string2Message("NOSUCH", &tc.client));
	CHECK_EQ(lastTo(srv, &tc.client),
		std::string(":CoolServ 421 * NOSUCH :Unknown command.\r\n"));
}

TEST(response_unknown_command)
{
	TestClient	tc;
	Message		msg = irc::string2Message("NOSUCH", &tc.client);

	CHECK_EQ(Response::handleNumeric(msg, irc::UNKNOWNCOMMAND),
		std::string(":CoolServ 421 * NOSUCH :Unknown command.\r\n"));
}

TEST(response_no_nickname_given)
{
	TestClient	tc;
	Message		msg = irc::string2Message("NICK", &tc.client);

	CHECK_EQ(Response::handleNumeric(msg, irc::NONICKNAMEGIVEN),
		std::string(":CoolServ 431 * :No nickname given.\r\n"));
}

TEST(response_nickname_in_use)
{
	TestClient	tc;
	Message		msg = irc::string2Message("NICK alice", &tc.client);

	CHECK_EQ(Response::handleNumeric(msg, irc::NICKNAMEINUSE),
		std::string(":CoolServ 433 * alice :Nickname is already in use.\r\n"));
}

TEST(response_erroneus_nickname)
{
	TestClient	tc;
	Message		msg = irc::string2Message("NICK #x", &tc.client);

	CHECK_EQ(Response::handleNumeric(msg, irc::ERRONEUSNICKNAME),
		std::string(":CoolServ 432 * #x :Erroneus nickname.\r\n"));
}

TEST(response_not_registered)
{
	TestClient	tc;
	Message		msg = irc::string2Message("PRIVMSG x :y", &tc.client);

	CHECK_EQ(Response::handleNumeric(msg, irc::NOTREGISTERED),
		std::string(":CoolServ 451 * :You have not registered\r\n"));
}

TEST(response_need_more_params)
{
	TestClient	tc;
	Message		msg = irc::string2Message("PASS", &tc.client);

	CHECK_EQ(Response::handleNumeric(msg, irc::NEEDMOREPARAMS),
		std::string(":CoolServ 461 * PASS :Not enough parameters.\r\n"));
}

TEST(response_already_registered)
{
	TestClient	tc;
	Message		msg = irc::string2Message("PASS x", &tc.client);

	CHECK_EQ(Response::handleNumeric(msg, irc::ALREADYREGISTERED),
		std::string(":CoolServ 462 * :You may not reregister.\r\n"));
}

TEST(response_password_mismatch)
{
	TestClient	tc;
	Message		msg = irc::string2Message("PASS x", &tc.client);

	CHECK_EQ(Response::handleNumeric(msg, irc::PASSWDMISMATCH),
		std::string(":CoolServ 464 * :Password incorrect.\r\n"));
}

TEST(registration_needs_all_three)
{
	CommandRegistry	reg;
	FakeServer		srv;
	TestClient		tc;

	reg.registerCmds();
	srv.password = "1o.0";
	reg.execute(srv, irc::string2Message("PASS 1o.0", &tc.client));
	reg.execute(srv, irc::string2Message("NICK alice", &tc.client));
	CHECK_EQ(srv.completeCalls, 0);
	reg.execute(srv, irc::string2Message("USER u 0 * :Real Name", &tc.client));
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
	reg.execute(srv, irc::string2Message("CAP LS", &tc.client));
	CHECK(tc.client.getCap());
	reg.execute(srv, irc::string2Message("PASS 1o.0", &tc.client));
	reg.execute(srv, irc::string2Message("NICK alice", &tc.client));
	reg.execute(srv, irc::string2Message("USER u 0 * :Real", &tc.client));
	CHECK_EQ(srv.completeCalls, 0);
	reg.execute(srv, irc::string2Message("CAP END", &tc.client));
	CHECK_EQ(srv.completeCalls, 1);
	CHECK(!tc.client.getCap());
}
