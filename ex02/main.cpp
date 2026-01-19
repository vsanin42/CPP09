/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vsanin <vsanin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/11 18:57:53 by vsanin            #+#    #+#             */
/*   Updated: 2026/01/19 16:27:10 by vsanin           ###   ########.fr       */
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
		p.setComparisons(0);
		p.FJMIEntry(p.getDeque());
		p.printContainers(AFTER, BOTH);
	}
	catch (const std::exception& e)
	{
		std::cerr << BRED << e.what() << '\n' << RESET;
		return 1;
	}
	return 0;
}
