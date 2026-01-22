/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vsanin <vsanin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/11 18:57:53 by vsanin            #+#    #+#             */
/*   Updated: 2026/01/22 18:06:11 by vsanin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "PmergeMe.hpp"
#include <iostream>
#include <iomanip>
#include <ctime>

int main(int argc, char** argv)
{
	if (argc < 2)
	{
		std::cerr << RED << "Error: at least one argument expected\n" << RESET;
		return 1;
	}
	PmergeMe p;
	try
	{
		clock_t start, end;
		p.fillContainers(argc, argv);
		
		// p.printContainerShort(p.getVector(), "Before", 1, 1);
		p.printContainer(p.getVector(), "Before", 1, 1); // Use this version for running the tester as it needs full container printed
		
		start = clock();
		p.FJMIEntry(p.getVector());
		end = clock();
		
		// p.printContainerShort(p.getVector(), "After", 1, 1);
		p.printContainer(p.getVector(), "After", 1, 1); // Use this version for running the tester as it needs full container printed
		
		std::cout << "Time to process a range of " << p.getSize() << " elements with std::vector:\t" << static_cast<double>(end - start) << " μs\n";

		start = clock();
		p.FJMIEntry(p.getDeque());
		end = clock();
		
		std::cout << "Time to process a range of " << p.getSize() << " elements with std::deque: \t" << static_cast<double>(end - start) << " μs\n";
		
		std::cout << "Number of comparisons: " << p.getComparisons() << "\n";
		if (!p.isSorted(p.getVector()))
			throw std::runtime_error("Error: container not sorted");
	}
	catch (const std::exception& e)
	{
		std::cerr << RED << e.what() << '\n' << RESET;
		return 1;
	}
	return 0;
}
