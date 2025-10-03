/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vsanin <vsanin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/11 18:57:53 by vsanin            #+#    #+#             */
/*   Updated: 2025/10/03 18:10:38 by vsanin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "RPN.hpp"
#include <iostream>
#include <stack>
#include <string>

int main(int argc, char** argv)
{
	if (argc != 2)
	{
		std::cerr << "Error: one argument expected\n";
		return 1;
	}
	RPN rpn;
	std::string arg(argv[1]);
	size_t len = arg.size();
	try
	{
		if (!len)
			throw std::invalid_argument("Error: empty argument");
		if (arg.find_first_not_of("0123456789+-*/ ") != std::string::npos)
			throw std::invalid_argument("Error: forbidden character found");
		rpn.solveRPN(arg, len);
	}
	catch (const std::exception& e)
	{
		std::cerr << e.what() << '\n';
		return 1;
	}
	return 0;
}
