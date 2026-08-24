/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   harness.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mweghofe <mweghofe@student.42vienna.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/24 10:48:00 by mweghofe          #+#    #+#             */
/*   Updated: 2026/08/24 10:48:00 by mweghofe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HARNESS_HPP
# define HARNESS_HPP

# include <iostream>
# include <string>
# include <vector>
# include <cstddef>

struct TestCase
{
	const char	*name;
	void		(*fn)();
};

inline std::vector<TestCase>	&testList()
{
	static std::vector<TestCase>	list;
	return (list);
}

inline int	&failCount()
{
	static int	n = 0;
	return (n);
}

struct TestReg
{
	TestReg(const char *name, void (*fn)())
	{
		TestCase	t;

		t.name = name;
		t.fn = fn;
		testList().push_back(t);
	}
};

# define TEST(name) \
	static void name(); \
	static TestReg _reg_##name(#name, name); \
	static void name()

# define CHECK(cond) \
	do \
	{ \
		if (!(cond)) \
		{ \
			++failCount(); \
			std::cerr << __FILE__ << ":" << __LINE__ \
				<< "  CHECK(" #cond ") failed\n"; \
		} \
	} while (0)

# define CHECK_EQ(a, b) \
	do \
	{ \
		if (!((a) == (b))) \
		{ \
			++failCount(); \
			std::cerr << __FILE__ << ":" << __LINE__ \
				<< "  CHECK_EQ(" #a ", " #b ") failed\n" \
				<< "    lhs: " << (a) << "\n" \
				<< "    rhs: " << (b) << "\n"; \
		} \
	} while (0)

# define CHECK_THROW(expr, exctype) \
	do \
	{ \
		bool	_threw = false; \
		try \
		{ \
			expr; \
		} \
		catch (const exctype &) \
		{ \
			_threw = true; \
		} \
		if (!_threw) \
		{ \
			++failCount(); \
			std::cerr << __FILE__ << ":" << __LINE__ \
				<< "  expected " #exctype " from " #expr "\n"; \
		} \
	} while (0)

#endif
