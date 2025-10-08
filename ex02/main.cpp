/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vsanin <vsanin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/11 18:57:53 by vsanin            #+#    #+#             */
/*   Updated: 2025/10/08 15:04:27 by vsanin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "PmergeMe.hpp"
#include <iostream>
#include <string>
#include <cstdlib>
#include <limits>
#include <vector>

void validateArgs(int argc, char** argv)
{
	std::string arg;
	for (int i = 1; i < argc; i++)
	{
		arg = argv[i];
		if (arg.find_first_not_of("0123456789") != std::string::npos)
			throw std::invalid_argument("Error: non-integer/negative argument found");
		double val = std::atof(arg.c_str());
		std::cout << val << "\n";
		if (val > std::numeric_limits<int>::max())
			throw std::invalid_argument("Error: argument out of integer value range");
	}
}

int main(int argc, char** argv)
{
	if (argc < 2)
	{
		std::cerr << "Error: at least one argument expected\n";
		return 1;
	}
	try
	{
		validateArgs(argc, argv);
		
	}
	catch (const std::exception& e)
	{
		std::cerr << e.what() << '\n';
		return 1;
	}
	
	return 0;
}
