/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PmergeMe.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vsanin <vsanin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/16 14:50:20 by vsanin            #+#    #+#             */
/*   Updated: 2026/01/07 17:38:49 by vsanin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "PmergeMe.hpp"
#include <iostream>
#include <string>
#include <cstdlib>
#include <limits>
#include <vector>
#include <cerrno>
#include <cmath>

/*================================[ OCF ]=====================================*/

PmergeMe::PmergeMe() : vec(), deq(), size(0), comparisons(0) {}

PmergeMe::PmergeMe(const PmergeMe& ref) : vec(ref.vec), deq(ref.deq), size(0), comparisons(0) {}

PmergeMe& PmergeMe::operator=(const PmergeMe& ref)
{
	if (this != &ref)
	{
		vec = ref.vec;
		deq = ref.deq;
		size = ref.size;
		comparisons = ref.comparisons;
	}
	return *this;
}

PmergeMe::~PmergeMe() {}

/*===============================[ Inits ]====================================*/

int PmergeMe::validateArg(const char* arg)
{
	if (arg[0] == '\0')
		throw std::invalid_argument("Error: empty argument");
	std::string orig(arg);
	if (arg[0] == '+' && arg[1] != '\0')
		arg++;
	std::string str(arg);
	if (str.find_first_not_of("0123456789") != std::string::npos)
		throw std::invalid_argument("Error: non-digit character found in argument (" + orig + ")");
	
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
	size = vec.size();
}

/*===============================[ Utils ]====================================*/

void PmergeMe::printContainers(PrintWhenOptions whenOption, PrintWhichOptions whichOption)
{
	switch (whenOption)
	{
		case BEFORE:
			std::cout << "Before sorting:\n";
			break;
		case AFTER:
			std::cout << "After sorting:\n";
			break;
		case NONE:
			break;
		default:
			std::cerr << "Invalid 'when' option.\n";
			return;
	}
	
	switch (whichOption)
	{
		case BOTH:
			std::cout << "Vector:\t";
			for (std::vector<int>::iterator it = vec.begin(); it != vec.end(); ++it)
				std::cout << " " << *it;
			std::cout << "\nDeque:\t";
			for (std::deque<int>::iterator it = deq.begin(); it != deq.end(); ++it)
				std::cout << " " << *it;
			break;
		case VECTOR:
			std::cout << "Vector:\t";
			for (std::vector<int>::iterator it = vec.begin(); it != vec.end(); ++it)
				std::cout << " " << *it;
			break;
		case DEQUE:
			std::cout << "Deque:\t";
			for (std::deque<int>::iterator it = deq.begin(); it != deq.end(); ++it)
				std::cout << " " << *it;
			break;
		default:
			std::cerr << "Invalid 'which' option.";
	}
	std::cout << "\n";
}

/*===============================[ Algo ]=====================================*/

std::vector<int> PmergeMe::sortPairs(size_t compFactor, size_t pairNum)
{
	for (size_t i = compFactor * 2 - 1; i < size; i += compFactor * 2)
	{
		if (DEBUG) std::cout << "In for-loop. Pair: " << vec[i - compFactor] << " " << vec[i] << "\n";
		if (vec[i - compFactor] > vec[i])
		{
			if (DEBUG) std::cout << "Pair of elements above needs swapping based on rightmost number comparison.\n";

			size_t j = i - compFactor;
			size_t k = i;
			int temp = 0;
			while (k > i - compFactor)
			{
				temp = vec[j];
				vec[j] = vec[k];
				vec[k] = temp;
				j--;
				k--;
			}
		}
		comparisons++;
	}
	size_t isOdd = size % (compFactor * 2);
	if (DEBUG) std::cout << "\nIs there a remainder after size & compFactor? : " << isOdd << "\n";
	if (isOdd)
	{
		std::vector<int> remainder(vec.begin() + (compFactor * (2 * pairNum)), vec.end());
		if (DEBUG)
		{
			std::cout << "Remainder: ";
			for (std::vector<int>::iterator it = remainder.begin(); it != remainder.end(); ++it)
					std::cout << " " << *it;
			std::cout << "\n";
		}
		return remainder;
	}
	
	return std::vector<int>();
}

void PmergeMe::FJMI(size_t level)
{
	// 1. Setup info.
	size_t numsInPair = std::pow(2, level);
	size_t numsInElement = numsInPair / 2;
	size_t pairNum = size / numsInPair;
	size_t remainingNums = size - pairNum * numsInPair;
	
	if (DEBUG)
	{
		std::cout << "\n-------------------------------\n\n";
		std::cout << "Entering func call with " << level << " level\n";
		std::cout << "At this level there will be " << pairNum << " pairs. ";
		std::cout << "A pair has " << numsInPair << " numbers. ";
		std::cout << "An element in a pair has " << numsInElement << " numbers.\n";
		std::cout << "There will be " << remainingNums << " remaining numbers.\n\n";
	}


	
	// 2. Basic sorting
	size_t compFactor = std::pow(2, level - 1);
	if (DEBUG) std::cout << "Comparing numbers " << compFactor << " apart.\n";
	if (DEBUG) printContainers(BEFORE, VECTOR);
	
	std::vector<int> remainder = sortPairs(compFactor, pairNum);
	if (DEBUG) printContainers(AFTER, VECTOR);
	if (DEBUG) std::cout << "Comparisons after this level: " << comparisons << "\n";
	

	
	// 3. Recursion logic
	if (pairNum < 2)
	{
		if (DEBUG) std::cout << "\nThe next level will not be able to form pairs. Returning.\n";
		return;
	}
	FJMI(level + 1);
}
