/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_policies.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mweghofe <mweghofe@student.42vienna.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/24 10:48:00 by mweghofe          #+#    #+#             */
/*   Updated: 2026/08/24 10:48:00 by mweghofe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Command.hpp"
#include "Message.hpp"
#include "client_fixture.hpp"
#include "fake_server.hpp"
#include "ft_irc.hpp"
#include "harness.hpp"
#include "policies/AlreadyRegisteredPlcy.hpp"
#include "policies/ArgsLimitPlcy.hpp"

namespace
{
	int	g_dummy_calls = 0;

	int	dummy_cmd(IServerCtrl &, const Message &)
	{
		++g_dummy_calls;
		return (rfc::OK);
	}
}

TEST(args_limit_ok)
{
	FakeServer		srv;
	TestClient		tc;
	ArgsLimitPlcy	plcy(1, 1);
	Message			msg = string2Message("PASS secret", &tc.client);

	CHECK_EQ(plcy.check(msg, srv), rfc::OK);
}

TEST(args_limit_too_few)
{
	FakeServer		srv;
	TestClient		tc;
	ArgsLimitPlcy	plcy(1, 1);
	Message			msg = string2Message("PASS", &tc.client);

	CHECK_EQ(plcy.check(msg, srv), rfc::FEWPARAMS);
}

TEST(args_limit_too_many)
{
	FakeServer		srv;
	TestClient		tc;
	ArgsLimitPlcy	plcy(1, 1);
	Message			msg = string2Message("PASS a b", &tc.client);

	CHECK_EQ(plcy.check(msg, srv), rfc::MANYPARAMS);
}

TEST(already_registered_requires_done)
{
	FakeServer				srv;
	TestClient				tc;
	AlreadyRegisteredPlcy	need_reg(true);
	AlreadyRegisteredPlcy	need_unreg(false);

	CHECK_EQ(need_reg.check(string2Message("PRIVMSG x :y", &tc.client), srv),
		rfc::NOTREG);
	CHECK_EQ(need_unreg.check(string2Message("PASS x", &tc.client), srv),
		rfc::OK);

	tc.client.setRegistrationFlags(REG_DONE);
	CHECK_EQ(need_reg.check(string2Message("PRIVMSG x :y", &tc.client), srv),
		rfc::OK);
	CHECK_EQ(need_unreg.check(string2Message("PASS x", &tc.client), srv),
		rfc::ALREADYREG);
}

TEST(already_registered_partial_is_not_done)
{
	FakeServer				srv;
	TestClient				tc;
	AlreadyRegisteredPlcy	need_reg(true);

	tc.client.setRegistrationFlags(REG_PASSWD);
	CHECK_EQ(need_reg.check(string2Message("PRIVMSG x :y", &tc.client), srv),
		rfc::NOTREG);
}

TEST(command_policy_blocks_handler)
{
	FakeServer	srv;
	TestClient	tc;
	Command		cmd("FOO", dummy_cmd);

	g_dummy_calls = 0;
	cmd.addPolicy(new ArgsLimitPlcy(1, 1));
	CHECK_EQ(cmd.execute(srv, string2Message("FOO", &tc.client)),
		rfc::FEWPARAMS);
	CHECK_EQ(g_dummy_calls, 0);
	CHECK_EQ(cmd.execute(srv, string2Message("FOO bar", &tc.client)),
		rfc::OK);
	CHECK_EQ(g_dummy_calls, 1);
}
