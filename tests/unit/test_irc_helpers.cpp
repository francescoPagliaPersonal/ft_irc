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
