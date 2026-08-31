/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   harness.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mweghofe <mweghofe@student.42vienna.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/24 10:48:00 by mweghofe          #+#    #+#             */
/*   Updated: 2026/08/31 11:06:00 by mweghofe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HARNESS_HPP
# define HARNESS_HPP

# include <iostream>
# include <sstream>
# include <string>
# include <vector>
# include <cstddef>

struct TestFailure
{
	const char		*file;
	int				line;
	std::string		message;
};

struct TestCase
{
	const char		*name;
	const char		*file;
	void			(*fn)();
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

inline std::vector<TestFailure>	&failMessages()
{
	static std::vector<TestFailure>	msgs;
	return (msgs);
}

inline void	clearFailMessages()
{
	failMessages().clear();
}

inline void	recordFail(const char *file, int line, const std::string &message)
{
	TestFailure	f;

	f.file = file;
	f.line = line;
	f.message = message;
	failMessages().push_back(f);
}

struct TestReg
{
	TestReg(const char *name, const char *file, void (*fn)())
	{
		TestCase	t;

		t.name = name;
		t.file = file;
		t.fn = fn;
		testList().push_back(t);
	}
};

# define TEST(name) \
	static void name(); \
	static TestReg _reg_##name(#name, __FILE__, name); \
	static void name()

# define CHECK(cond) \
	do \
	{ \
		if (!(cond)) \
		{ \
			++failCount(); \
			recordFail(__FILE__, __LINE__, \
				std::string("CHECK(" #cond ") failed")); \
		} \
	} while (0)

# define CHECK_EQ(a, b) \
	do \
	{ \
		if (!((a) == (b))) \
		{ \
			++failCount(); \
			std::ostringstream	_oss; \
			_oss << "CHECK_EQ(" #a ", " #b ") failed\n" \
				<< "    lhs: " << (a) << "\n" \
				<< "    rhs: " << (b); \
			recordFail(__FILE__, __LINE__, _oss.str()); \
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
			recordFail(__FILE__, __LINE__, \
				std::string("expected " #exctype " from " #expr)); \
		} \
	} while (0)

#endif
