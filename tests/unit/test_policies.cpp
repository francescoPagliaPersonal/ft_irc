/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_policies.cpp                                  :+:      ::::::::   */
/*                                                    +:+ +:+         +:+     */
/*   By: mweghofe <mweghofe@student.42vienna.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/24 10:48:00 by mweghofe          #+#    #+#             */
/*   Updated: 2026/08/31 09:40:00 by mweghofe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Command.hpp"
#include "Message.hpp"
#include "client_fixture.hpp"
#include "fake_server.hpp"
#include "irc.hpp"
#include "harness.hpp"
#include "policies/AlreadyRegisteredPlcy.hpp"
#include "policies/ArgsLimitPlcy.hpp"

namespace
{
	int	g_dummy_calls = 0;

	rfc	dummy_cmd(IServerCtrl &, const Message &)
	{
		++g_dummy_calls;
		return (irc::OK);
	}
}

TEST(args_limit_ok)
{
	TestClient		tc;
	ArgsLimitPlcy	plcy(1, 1);
	Message			msg = irc::string2Message("PASS secret", &tc.client);

	CHECK_EQ(plcy.check(msg), irc::OK);
}

TEST(args_limit_too_few)
{
	TestClient		tc;
	ArgsLimitPlcy	plcy(1, 1);
	Message			msg = irc::string2Message("PASS", &tc.client);

	CHECK_EQ(plcy.check(msg), irc::NEEDMOREPARAMS);
}

TEST(args_limit_too_many)
{
	TestClient		tc;
	ArgsLimitPlcy	plcy(1, 1);
	Message			msg = irc::string2Message("PASS a b", &tc.client);

	CHECK_EQ(plcy.check(msg), irc::MANYPARAMS);
}

TEST(already_registered_requires_done)
{
	TestClient				tc;
	AlreadyRegisteredPlcy	need_reg(true);
	AlreadyRegisteredPlcy	need_unreg(false);

	CHECK_EQ(need_reg.check(irc::string2Message("PRIVMSG x :y", &tc.client)),
		irc::NOTREGISTERED);
	CHECK_EQ(need_unreg.check(irc::string2Message("PASS x", &tc.client)),
		irc::OK);

	tc.client.setRegistrationFlags(REG_DONE);
	CHECK_EQ(need_reg.check(irc::string2Message("PRIVMSG x :y", &tc.client)),
		irc::OK);
	CHECK_EQ(need_unreg.check(irc::string2Message("PASS x", &tc.client)),
		irc::ALREADYREGISTERED);
}

TEST(already_registered_partial_is_not_done)
{
	TestClient				tc;
	AlreadyRegisteredPlcy	need_reg(true);

	tc.client.setRegistrationFlags(REG_PASSWD);
	CHECK_EQ(need_reg.check(irc::string2Message("PRIVMSG x :y", &tc.client)),
		irc::NOTREGISTERED);
}

TEST(command_policy_blocks_handler)
{
	FakeServer	srv;
	TestClient	tc;
	Command		cmd("FOO", dummy_cmd);

	g_dummy_calls = 0;
	cmd.addPolicy(new ArgsLimitPlcy(1, 1));
	CHECK_EQ(cmd.execute(srv, irc::string2Message("FOO", &tc.client)),
		irc::NEEDMOREPARAMS);
	CHECK_EQ(g_dummy_calls, 0);
	CHECK_EQ(cmd.execute(srv, irc::string2Message("FOO bar", &tc.client)),
		irc::OK);
	CHECK_EQ(g_dummy_calls, 1);
}

TEST(args_limit_trailing_counts)
{
	TestClient		tc;
	ArgsLimitPlcy	plcy(2, 2);
	Message			ok = irc::string2Message("FOO dest :text", &tc.client);
	Message			few = irc::string2Message("FOO :text", &tc.client);

	CHECK_EQ(plcy.check(ok), irc::OK);
	CHECK_EQ(plcy.check(few), irc::NEEDMOREPARAMS);
}

TEST(command_registered_policy_blocks_before_args)
{
	FakeServer	srv;
	TestClient	tc;
	Command		cmd("PRIV", dummy_cmd);

	g_dummy_calls = 0;
	cmd.addPolicy(new AlreadyRegisteredPlcy(true));
	cmd.addPolicy(new ArgsLimitPlcy(2, 2));
	CHECK_EQ(cmd.execute(srv, irc::string2Message("PRIV", &tc.client)),
		irc::NOTREGISTERED);
	CHECK_EQ(g_dummy_calls, 0);
}
