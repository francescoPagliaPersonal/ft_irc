/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_channel.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mweghofe <mweghofe@student.42vienna.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/31 10:33:00 by mweghofe          #+#    #+#             */
/*   Updated: 2026/08/31 10:33:00 by mweghofe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Channel.hpp"
#include "client_fixture.hpp"
#include "fake_server.hpp"
#include "ft_irc.hpp"
#include "harness.hpp"
#include "test_cmd_helper.hpp"

#include <string>

TEST(channel_title_compliant)
{
	CHECK(Channel::isTitleCompliant("#abc"));
	CHECK(Channel::isTitleCompliant("&room"));
	CHECK(!Channel::isTitleCompliant("#ab"));
	CHECK(!Channel::isTitleCompliant("nope"));
	CHECK(!Channel::isTitleCompliant(
		"#abcdefghijabcdefghijabcdefghijab"));
}

TEST(channel_title2key)
{
	CHECK_EQ(Channel::title2key("#Foo"), std::string("FOO"));
}

TEST(channel_membership_and_op)
{
	TestClient	tc;
	Channel		ch("#test", "");

	ch.addClient(&tc.client, US_OPERATOR);
	CHECK(ch.isMember(&tc.client));
	CHECK(ch.isChanOp(&tc.client));
	CHECK(!ch.isEmpty());
	ch.removeClient(&tc.client);
	CHECK(ch.isEmpty());
	CHECK(!ch.isMember(&tc.client));
}

TEST(channel_password_match)
{
	Channel	ch("#keyed", "secret");

	CHECK(ch.passwordMatch("secret"));
	CHECK(!ch.passwordMatch("wrong"));
	Channel	empty("#open", "");
	CHECK(empty.passwordMatch(""));
	CHECK(empty.passwordMatch("x"));
}

TEST(channel_limit_fullness)
{
	TestClient	a;
	TestClient	b;
	Channel		ch("#lim", "");

	ch.setLimit(2);
	ch.addClient(&a.client, US_BASIC);
	CHECK(ch.belowChannelLimit());
	ch.addClient(&b.client, US_BASIC);
	CHECK(!ch.belowChannelLimit());
}

TEST(channel_join_granted_default)
{
	TestClient	tc;
	Channel		ch("#test", "");

	CHECK(ch.joinGranted(&tc.client));
	ch.invite(&tc.client);
	CHECK(ch.joinGranted(&tc.client));
}

TEST(channel_topic_default_and_clear)
{
	Channel	ch("#test", "");

	CHECK_EQ(ch.getTopic(),
		std::string("Welcome to this beautiful channel!"));
	ch.setTopic("");
	CHECK_EQ(ch.getTopic(), std::string(""));
}

TEST(fake_add_to_channel_creates_founder_op)
{
	FakeServer	srv;
	TestClient	tc;

	registerClient(srv, tc, "alice");
	CHECK_EQ(srv.addToChannel(&tc.client, "#chan", ""), irc::OK);
	Channel	*ch = srv.getChannelByTitle("#chan");

	CHECK(ch != 0);
	CHECK(ch->isMember(&tc.client));
	CHECK(ch->isChanOp(&tc.client));
}

TEST(fake_add_to_channel_bad_title)
{
	FakeServer	srv;
	TestClient	tc;

	registerClient(srv, tc, "alice");
	CHECK_EQ(srv.addToChannel(&tc.client, "#ab", ""), irc::BADCHANMASK);
	CHECK(srv.getChannelByTitle("#ab") == 0);
}

TEST(fake_add_to_channel_duplicate)
{
	FakeServer	srv;
	TestClient	tc;

	registerClient(srv, tc, "alice");
	CHECK_EQ(srv.addToChannel(&tc.client, "#chan", ""), irc::OK);
	CHECK_EQ(srv.addToChannel(&tc.client, "#chan", ""),
		irc::USERONCHANNEL);
}

/* needs MODE / getLimit
TEST(channel_get_limit_default)
{
	Channel ch("#test", "");

	CHECK_EQ(ch.getLimit(), static_cast<irc::uint>(0));
}

TEST(channel_get_limit_after_set)
{
	Channel ch("#test", "");

	ch.setLimit(10);
	CHECK_EQ(ch.getLimit(), static_cast<irc::uint>(10));
}
*/
