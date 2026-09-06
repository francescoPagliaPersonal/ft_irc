/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mweghofe <mweghofe@student.42vienna.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/24 10:48:00 by mweghofe          #+#    #+#             */
/*   Updated: 2026/08/31 11:06:00 by mweghofe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "harness.hpp"

#include <iostream>
#include <map>
#include <string>
#include <vector>

namespace
{
	const char	*SUITE_ORDER[] = {
		"args",
		"message",
		"policies",
		"commands",
		"irc_helpers",
		"channel",
		"join",
		"invite",
		"mode",
		"quit",
		"framing",
		0
	};

	std::string	basename(const char *path)
	{
		std::string	s(path);
		std::string::size_type	pos;

		pos = s.find_last_of('/');
		if (pos != std::string::npos)
			s = s.substr(pos + 1);
		pos = s.find_last_of('\\');
		if (pos != std::string::npos)
			s = s.substr(pos + 1);
		return (s);
	}

	std::string	suiteTitle(const char *file)
	{
		std::string	title;

		title = basename(file);
		if (title.size() > 5 && title.compare(0, 5, "test_") == 0)
			title = title.substr(5);
		if (title.size() > 4
			&& title.compare(title.size() - 4, 4, ".cpp") == 0)
			title = title.substr(0, title.size() - 4);
		return (title);
	}

	void	printRule()
	{
		std::cout << testout::rule() << "°°°°°°°°°°°°"
			<< testout::reset() << "\n";
	}

	void	printBanner()
	{
		std::cout << testout::banner() << "   "
			<< testout::title() << testout::bold()
			<< "ft_irc" << testout::reset()
			<< testout::banner() << "  unit tests"
			<< testout::reset() << "\n";
		printRule();
		std::cout << "\n";
	}

	void	printFailureDetails(const std::vector<TestFailure> &failures)
	{
		for (size_t i = 0; i < failures.size(); ++i)
		{
			std::cout << testout::dim() << testout::fail()
				<< "        " << failures[i].file << ":"
				<< failures[i].line << "  " << failures[i].message
				<< testout::reset();
			if (!failures[i].message.empty()
				&& failures[i].message[failures[i].message.size() - 1] != '\n')
				std::cout << "\n";
		}
	}

	bool	runTest(const TestCase &test, int &suite_failed)
	{
		int	before;

		clearFailMessages();
		before = failCount();
		test.fn();
		if (failCount() != before)
		{
			++suite_failed;
			std::cout << "  " << testout::fail() << "FAIL"
				<< testout::reset() << "  " << test.name << "\n";
			printFailureDetails(failMessages());
			return (false);
		}
		std::cout << "  " << testout::pass() << "PASS"
			<< testout::reset() << "  " << test.name << "\n";
		return (true);
	}

	void	runSuite(const std::string &title,
		const std::vector<const TestCase *> &cases,
		int &total_failed)
	{
		int	suite_failed;
		int	suite_passed;

		if (cases.empty())
			return ;
		std::cout << testout::banner() << "── "
			<< testout::title() << title
			<< testout::banner() << "  (" << cases.size() << ") ──"
			<< testout::reset() << "\n";
		suite_failed = 0;
		for (size_t i = 0; i < cases.size(); ++i)
			runTest(*cases[i], suite_failed);
		suite_passed = static_cast<int>(cases.size()) - suite_failed;
		total_failed += suite_failed;
		std::cout << "  " << suite_passed << " passed, "
			<< suite_failed << " failed\n\n";
	}

	int	suiteOrderIndex(const std::string &title)
	{
		for (size_t i = 0; SUITE_ORDER[i] != 0; ++i)
		{
			if (title == SUITE_ORDER[i])
				return (static_cast<int>(i));
		}
		return (1000);
	}

	void	buildSuites(const std::vector<TestCase> &tests,
		std::map<std::string, std::vector<const TestCase *> > &suites)
	{
		for (size_t i = 0; i < tests.size(); ++i)
		{
			std::string	title;

			title = suiteTitle(tests[i].file);
			suites[title].push_back(&tests[i]);
		}
	}

	void	printSuites(const std::map<std::string, std::vector<const TestCase *> > &suites,
		int &total_failed, size_t &total_tests)
	{
		std::vector<std::string>				ordered;
		std::map<std::string, std::vector<const TestCase *> >::const_iterator	it;

		for (it = suites.begin(); it != suites.end(); ++it)
			ordered.push_back(it->first);
		for (size_t i = 0; i < ordered.size(); ++i)
		{
			size_t	best;

			best = i;
			for (size_t j = i + 1; j < ordered.size(); ++j)
			{
				if (suiteOrderIndex(ordered[j]) < suiteOrderIndex(ordered[best]))
					best = j;
			}
			if (best != i)
			{
				std::string	tmp;

				tmp = ordered[i];
				ordered[i] = ordered[best];
				ordered[best] = tmp;
			}
		}
		for (size_t i = 0; i < ordered.size(); ++i)
		{
			total_tests += suites.find(ordered[i])->second.size();
			runSuite(ordered[i], suites.find(ordered[i])->second, total_failed);
		}
	}
}

int	main()
{
	const std::vector<TestCase>							&tests = testList();
	std::map<std::string, std::vector<const TestCase *> >	suites;
	int													total_failed;
	size_t												total_tests;

	total_failed = 0;
	total_tests = 0;
	buildSuites(tests, suites);
	printBanner();
	printSuites(suites, total_failed, total_tests);
	printRule();
	if (total_failed == 0)
	{
		std::cout << testout::pass() << "  "
			<< (total_tests - static_cast<size_t>(total_failed))
			<< " passed, " << total_failed << " failed, "
			<< total_tests << " total" << testout::reset() << "\n";
	}
	else
	{
		std::cout << testout::fail() << "  "
			<< (total_tests - static_cast<size_t>(total_failed))
			<< " passed, " << total_failed << " failed, "
			<< total_tests << " total" << testout::reset() << "\n";
	}
	return (failCount() ? 1 : 0);
}
