/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PmergeMe.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vsanin <vsanin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/16 14:50:20 by vsanin            #+#    #+#             */
/*   Updated: 2026/01/06 14:50:48 by vsanin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "PmergeMe.hpp"
#include <iostream>
#include <string>
#include <cstdlib>
#include <limits>
#include <vector>
#include <cerrno>

/*================================[ OCF ]=====================================*/

PmergeMe::PmergeMe() : vec(), deq() {}

PmergeMe::PmergeMe(const PmergeMe& ref) : vec(ref.vec), deq(ref.deq) {}

PmergeMe& PmergeMe::operator=(const PmergeMe& ref)
{
	if (this != &ref)
	{
		vec = ref.vec;
		deq = ref.deq;
	}
	return *this;
}

PmergeMe::~PmergeMe() {}

/*===============================[ Inits ]====================================*/

int PmergeMe::validateArg(const char* arg)
{
	std::string orig(arg);
	if (arg[0] == '+' && arg[1] != '\0')
		arg++;
	std::string str(arg);
	if (str.find_first_not_of("0123456789") != std::string::npos)
		throw std::invalid_argument("Error: non-integer/negative argument found (" + orig + ")");
	
	errno = 0;
	long l = std::strtol(str.c_str(), 0, 10);
	if (errno == ERANGE || l > std::numeric_limits<int>::max())
		throw std::invalid_argument("Error: argument out of range (" + orig + ")");
	
	return static_cast<int>(l);
}

void PmergeMe::fillContainers(int argc, char** argv)
{
	int valid = 0;
	std::cout << "Receiving arguments...\n";
	for (int i = 1; i < argc; i++)
	{
		valid = PmergeMe::validateArg(argv[i]);
		vec.push_back(valid);
		deq.push_back(valid);
	}
}

/*===============================[ Utils ]====================================*/

void PmergeMe::printContainers(char option)
{
	switch (option)
	{
		case 'b':
			std::cout << "Containers before sorting:\n";
			break;
		case 'a':
			std::cout << "Containers after sorting:\n";
			break;
		default:
			std::cerr << "Invalid print option:\n";
			return;
	}
	
	std::cout << "Vector:\t";
	for (std::vector<int>::iterator it = vec.begin(); it != vec.end(); ++it)
		std::cout << " " << *it;
	std::cout << "\n";
	
	std::cout << "Deque:\t";
	for (std::deque<int>::iterator it = deq.begin(); it != deq.end(); ++it)
		std::cout << " " << *it;
	std::cout << "\n";
}
