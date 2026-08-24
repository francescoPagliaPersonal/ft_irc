/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_args.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mweghofe <mweghofe@student.42vienna.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/24 10:48:00 by mweghofe          #+#    #+#             */
/*   Updated: 2026/08/24 10:48:00 by mweghofe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "args.hpp"
#include "harness.hpp"

#include <stdexcept>
#include <string>

TEST(arg2port_valid)
{
	CHECK_EQ(arg2port("6667"), 6667);
	CHECK_EQ(arg2port("1024"), 1024);
	CHECK_EQ(arg2port("65535"), 65535);
}

TEST(arg2port_privileged)
{
	CHECK_THROW(arg2port("80"), std::out_of_range);
	CHECK_THROW(arg2port("1023"), std::out_of_range);
	CHECK_THROW(arg2port("0"), std::out_of_range);
}

TEST(arg2port_overflow)
{
	CHECK_THROW(arg2port("65536"), std::out_of_range);
	CHECK_THROW(arg2port("99999"), std::out_of_range);
}

TEST(arg2port_garbage)
{
	CHECK_THROW(arg2port("abc"), std::invalid_argument);
	CHECK_THROW(arg2port("6667foo"), std::invalid_argument);
	CHECK_THROW(arg2port(""), std::invalid_argument);
}

TEST(arg2password_valid)
{
	CHECK_EQ(arg2password("1o.0"), std::string("1o.0"));
	CHECK_EQ(arg2password("ab1."), std::string("ab1."));
}

TEST(arg2password_too_short)
{
	CHECK_THROW(arg2password("a1."), std::out_of_range);
	CHECK_THROW(arg2password(""), std::out_of_range);
}

TEST(arg2password_missing_classes)
{
	CHECK_THROW(arg2password("abcd"), std::invalid_argument);
	CHECK_THROW(arg2password("1234"), std::invalid_argument);
	CHECK_THROW(arg2password("...."), std::invalid_argument);
	CHECK_THROW(arg2password("ab12"), std::invalid_argument);
	CHECK_THROW(arg2password("ab.."), std::invalid_argument);
	CHECK_THROW(arg2password("12.."), std::invalid_argument);
}
