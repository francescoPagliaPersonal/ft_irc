/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mweghofe <mweghofe@student.42vienna.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/24 10:48:00 by mweghofe          #+#    #+#             */
/*   Updated: 2026/08/24 10:48:00 by mweghofe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "harness.hpp"

int	main()
{
	const std::vector<TestCase>	&tests = testList();
	int							failed_tests = 0;

	for (size_t i = 0; i < tests.size(); ++i)
	{
		int	before = failCount();

		tests[i].fn();
		if (failCount() != before)
		{
			++failed_tests;
			std::cerr << "[FAIL] " << tests[i].name << "\n";
		}
		else
			std::cout << "[PASS] " << tests[i].name << "\n";
	}
	std::cout << (tests.size() - static_cast<size_t>(failed_tests))
		<< " passed, " << failed_tests << " failed, "
		<< tests.size() << " total\n";
	return (failCount() ? 1 : 0);
}
