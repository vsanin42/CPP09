/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PmergeMe.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vsanin <vsanin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/16 14:50:20 by vsanin            #+#    #+#             */
/*   Updated: 2026/01/08 15:54:35 by vsanin           ###   ########.fr       */
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
	if (DEBUG) std::cout << "Receiving arguments...\n";
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
			printContainer(vec, "Vector", 2);
			printContainer(deq, "Deque", 2);
			break;
		case VECTOR:
			printContainer(vec, "Vector", 2);
			break;
		case DEQUE:
			printContainer(deq, "Deque", 2);
			break;
		default:
			std::cerr << "Invalid 'which' option.";
	}
}

/*===============================[ Algo ]=====================================*/

std::vector<int> PmergeMe::sortPairs(size_t compFactor, size_t pairNum)
{
	for (size_t i = compFactor * 2 - 1; i < size; i += compFactor * 2)
	{
		if (DEBUG) std::cout << "Checking - Pair: " << vec[i - compFactor] << " " << vec[i];
		if (vec[i - compFactor] > vec[i])
		{
			if (DEBUG) std::cout << " - " << RED << "Needs swapping." << RESET;

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
		if (DEBUG) std::cout << "\n";
		comparisons++;
	}
	size_t isOdd = size % (compFactor * 2);
	if (DEBUG) std::cout << "\nIs there a remainder after size & compFactor? : " << (isOdd ? GREEN : RED) << (isOdd ? "YES" : "NO") << "\n" << RESET;
	if (isOdd)
	{
		std::vector<int> remainder(vec.begin() + (compFactor * (2 * pairNum)), vec.end());
		if (DEBUG)
		{
			std::cout << MAGENTA;
			printContainer(remainder, "Remainder", 1);
			std::cout << RESET;
		}
		return remainder;
	}
	
	return std::vector<int>();
}

void PmergeMe::initMainPend(std::vector<int>& main, std::vector<int>& pend, size_t compFactor, size_t pairNum)
{
	if (pairNum > 1)
	{
		size_t i = compFactor * 2;
		for (size_t count = 1; count <= (size / compFactor) - 2; count++)
		{
			for (size_t j = 0; j < compFactor; j++)
			{
				count % 2 == 0 ? main.push_back(vec[i]) : pend.push_back(vec[i]);
				i++;
			}
		}
	}
}

void PmergeMe::FJMI(size_t level)
{
	// 1. Setup info.
	size_t numsInPair = std::pow(2, level);
	size_t numsInElement = numsInPair / 2;
	size_t pairNum = size / numsInPair;
	size_t remainingNums = size - pairNum * numsInPair;
	
	if (pairNum < 1)
	{
		if (DEBUG)
		{
			std::cout << "\n----------------------------------------\n";
			std::cout << BOLD YELLOW << "\nPairs cannnot be formed at this level (" << level << "). Returning.\n" << RESET;
			std::cout << "\n----------------------------------------\n";

			std::cout << BOLD << BBLUE
			<< "\n----------------------------------------"
			<< "\n[ STEP 2: Initiating 'main' and 'pend' ]"
			<< "\n----------------------------------------\n" << RESET;
		}
		return;
	}
	
	if (DEBUG)
	{
		if (level == 1)
			std::cout << BOLD << BBLUE
			<< "\n----------------------------------------"
			<< "\n[        STEP 1: Sorting pairs         ]"
			<< "\n----------------------------------------\n" << RESET;
		std::cout << "\n----------------------------------------\n\n";
		std::cout << BOLD YELLOW<< "Entering function call with LEVEL " << BCYAN << level << RESET << ".\n";
		std::cout << "At this level there will be " << BCYAN << pairNum << RESET << " pairs. ";
		std::cout << "A pair has " << BCYAN << numsInPair << RESET << " numbers. ";
		std::cout << "An element in a pair has " << BCYAN << numsInElement << RESET << " numbers. ";
		std::cout << "There will be " << BCYAN << remainingNums << RESET << " remaining numbers.\n\n";
	}

	// 2. Basic sorting
	size_t compFactor = numsInElement;
	
	if (DEBUG) printContainers(BEFORE, VECTOR);
	std::vector<int> remainder = sortPairs(compFactor, pairNum);
	if (DEBUG)
	{
		std::cout << GREEN;
		printContainers(AFTER, VECTOR);
		std::cout << RESET;
	}
	
	// 3. Recursion logic
	FJMI(level + 1);

	// 4. After recursion returns, insertion
	// 4.1. Initialize main and pend.
	std::vector<int> main(vec.begin(), vec.begin() + (compFactor * 2));
	std::vector<int> pend;

	initMainPend(main, pend, compFactor, pairNum);

	if (DEBUG)
	{
		std::cout << BOLD YELLOW << "\nLevel: " << BCYAN << level << RESET
				  << BOLD << ", compFactor: " << BCYAN << compFactor << RESET
				  << BOLD << ", pairNum: " << BCYAN << pairNum << "\n" << RESET;
		printContainer(vec, "Vector", 3);
		printContainer(main, "Main (b1, a1 & a's)", 1);
		printContainer(pend, "Pend (b's from b2)", 1);
		if (level == 1)
			std::cout << "\n----------------------------------------\n\n";
	}

	// 4.2. Insert ???
	
}
