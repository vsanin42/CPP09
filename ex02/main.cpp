/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vsanin <vsanin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/11 18:57:53 by vsanin            #+#    #+#             */
/*   Updated: 2026/01/20 14:28:34 by vsanin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "PmergeMe.hpp"
#include <iostream>

int main(int argc, char** argv)
{
	if (argc < 2)
	{
		std::cerr << BRED << "Error: at least one argument expected\n" << RESET;
		return 1;
	}
	PmergeMe p;
	try
	{
		p.fillContainers(argc, argv);
		p.printContainers(BEFORE, BOTH);
		p.FJMIEntry(p.getVector());
		if (DEBUG)
		{
			std::cout << "\nComparisons: " << p.getComparisons() << "\n";
			std::cout << "\n----------------------------------------\n\n";
		}
		p.setComparisons(0);
		p.FJMIEntry(p.getDeque());
		if (DEBUG)
		{
			std::cout << "\nComparisons: " << p.getComparisons() << "\n";
			std::cout << "\n----------------------------------------\n\n";
		}
		p.printContainers(AFTER, BOTH);
	}
	catch (const std::exception& e)
	{
		std::cerr << BRED << e.what() << '\n' << RESET;
		return 1;
	}
	return 0;
}
