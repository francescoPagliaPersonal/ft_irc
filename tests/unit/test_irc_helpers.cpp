/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_irc_helpers.cpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mweghofe <mweghofe@student.42vienna.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/31 10:33:00 by mweghofe          #+#    #+#             */
/*   Updated: 2026/08/31 10:33:00 by mweghofe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "harness.hpp"
#include "ft_irc.hpp"
#include "irc.hpp"

#include <string>
#include <vector>

TEST(strsplit_join_channels)
{
	std::vector<std::string>	parts;

	parts = irc::strSplit("#a,#b,#c", ',', false);
	CHECK_EQ(parts.size(), 3u);
	CHECK_EQ(parts[0], std::string("#a"));
	CHECK_EQ(parts[1], std::string("#b"));
	CHECK_EQ(parts[2], std::string("#c"));
}

TEST(strsplit_join_keys_keep_empty)
{
	std::vector<std::string>	parts;

	parts = irc::strSplit("key,,other", ',', true);
	CHECK_EQ(parts.size(), 3u);
	CHECK_EQ(parts[0], std::string("key"));
	CHECK_EQ(parts[1], std::string(""));
	CHECK_EQ(parts[2], std::string("other"));
}

TEST(is_name_compliant_nick_rules)
{
	CHECK(irc::isNameCompliant("alice"));
	CHECK(irc::isNameCompliant(""));
	CHECK(irc::isNameCompliant("Alice32"));
	CHECK(irc::isNameCompliant(std::string(32, 'a')));
	CHECK(!irc::isNameCompliant("a.b"));
	CHECK(!irc::isNameCompliant("a*b"));
	CHECK(!irc::isNameCompliant("#nick"));
	CHECK(!irc::isNameCompliant("*star"));
	CHECK(!irc::isNameCompliant(
		"aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"));
}

TEST(strsplit_drops_empty_when_not_kept)
{
	std::vector<std::string>	parts;

	parts = irc::strSplit(",#a,#b,", ',', false);
	CHECK_EQ(parts.size(), 3u);
	CHECK_EQ(parts[0], std::string(""));
	CHECK_EQ(parts[1], std::string("#a"));
	CHECK_EQ(parts[2], std::string("#b"));
}

TEST(strsplit_no_delimiter)
{
	std::vector<std::string>	parts;

	parts = irc::strSplit("#only", ',', false);
	CHECK_EQ(parts.size(), 1u);
	CHECK_EQ(parts[0], std::string("#only"));
}

TEST(strsplit_double_comma_kept)
{
	std::vector<std::string>	parts;

	parts = irc::strSplit("a,,b", ',', true);
	CHECK_EQ(parts.size(), 3u);
	CHECK_EQ(parts[1], std::string(""));
}

TEST(rfc_numeric_values)
{
	CHECK_EQ(static_cast<int>(irc::NOTOPIC), 331);
	CHECK_EQ(static_cast<int>(irc::CANNOTSENDTOCHAN), 404);
	CHECK_EQ(static_cast<int>(irc::ALREADYREGISTERED), 462);
}

TEST(chunkify_trailing_short)
{
	std::string	out;

	out = irc::chunkifyTrailing(":prefix :", "hello");
	CHECK_EQ(out, std::string(":prefix :hello\r\n"));
}

TEST(chunkify_trailing_splits_on_space)
{
	std::string	prefix;
	std::string	body;
	std::string	out;
	size_t		maxTrail;

	prefix = std::string(100, 'P');
	maxTrail = MSG_MAX_LENGTH - prefix.size() - 2;
	body = std::string(maxTrail - 10, 'x');
	body += " word2";
	out = irc::chunkifyTrailing(prefix, body);
	CHECK(out.find("\r\n") != std::string::npos);
	CHECK(out.size() > prefix.size() + body.size());
}

TEST(chunkify_trailing_hard_cut_without_spaces)
{
	std::string	prefix;
	std::string	body;
	std::string	out;
	size_t		maxTrail;

	prefix = std::string(100, 'P');
	maxTrail = MSG_MAX_LENGTH - prefix.size() - 2;
	body = std::string(maxTrail + 50, 'x');
	out = irc::chunkifyTrailing(prefix, body);
	CHECK(out.find("\r\n") != std::string::npos);
	CHECK(out.size() >= MSG_MAX_LENGTH);
}

TEST(chunkify_trailing_prefix_too_long)
{
	std::string	prefix;
	std::string	out;

	prefix = std::string(MSG_MAX_LENGTH - MIN_TRAIL_LENGTH, 'P');
	out = irc::chunkifyTrailing(prefix, "x");
	CHECK(out.empty());
}
