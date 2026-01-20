/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PmergeMe.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vsanin <vsanin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/16 14:50:20 by vsanin            #+#    #+#             */
/*   Updated: 2026/01/20 14:01:28 by vsanin           ###   ########.fr       */
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

/*============================================================================*/
/*||                                 OCF                                    ||*/
/*============================================================================*/

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

/*============================================================================*/
/*||                                INITS                                   ||*/
/*============================================================================*/

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

/*============================================================================*/
/*||                                UTILS                                   ||*/
/*============================================================================*/

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
			printContainer(vec, "Vector", 2, 1);
			printContainer(deq, "Deque", 2, 1);
			break;
		case VECTOR:
			printContainer(vec, "Vector", 2, 1);
			break;
		case DEQUE:
			printContainer(deq, "Deque", 2, 1);
			break;
		default:
			std::cerr << "Invalid 'which' option.";
	}
}

std::vector<int>& PmergeMe::getVector(void) { return vec; }

std::deque<int>& PmergeMe::getDeque(void) { return deq; }

void PmergeMe::setComparisons(size_t count) { comparisons = count; }

size_t PmergeMe::getComparisons(void) { return comparisons; }

/*============================================================================*/
/*||                             JACOBSTAHL                                 ||*/
/*============================================================================*/

// Offset, starts at 3 for n == 0
size_t PmergeMe::jacobstahlGenerator(size_t n)
{
	return (std::pow(2, n + 3) + std::pow(-1, n)) / 3;
}

void PmergeMe::jacobstahlCreate(std::vector<size_t>& jacobstahlSequence, size_t pendElementsCount)
{
	if (DEBUG) std::cout << BOLD << "Generating Jacobstahl sequence for the current pend element size ("
						 << BCYAN << pendElementsCount << RESET BOLD << "): \n" << RESET BBLUE;
	size_t n = 0;
	while (1)
	{
		if (!pendElementsCount || (n && jacobstahlSequence[n - 1] >= pendElementsCount))
			break;
		jacobstahlSequence.push_back(jacobstahlGenerator(n));
		n++;
	}
	if (DEBUG)
	{
		printContainer(jacobstahlSequence, "J", 0, 1);
		std::cout << RESET;
	}
}

/*============================================================================*/
/*||                         FJMI - ENTRY POINT                             ||*/
/*============================================================================*/

void PmergeMe::FJMIEntry(std::vector<int>& cont)
{
	std::vector< std::pair<std::vector<int>, size_t> > mainElements;

	if (DEBUG)
	{
		std::cout << BOLD << BBLUE
				  << "----------------------------------------"
				  << "\n|                                      |"
				  << "\n[     FORD-JOHNSON MERGE INSERTION     |"
				  << "\n|                                      |"
				  << "\n----------------------------------------\n\n" << RESET;
		std::cout << "Initiating algorithm with " << BOLD BGREEN << "std::vector" << RESET << "...\n";
	}
	FJMICore(cont, mainElements, 1);
}

void PmergeMe::FJMIEntry(std::deque<int>& cont)
{
	std::deque< std::pair<std::deque<int>, size_t> > mainElements;
	if (DEBUG)
	{
		std::cout << BOLD << BBLUE
				  << "----------------------------------------"
				  << "\n|                                      |"
				  << "\n[     FORD-JOHNSON MERGE INSERTION     |"
				  << "\n|                                      |"
				  << "\n----------------------------------------\n\n" << RESET;
		std::cout << "Initiating algorithm with " << BOLD BGREEN << "std::deque" << RESET << "...\n";
	}
	FJMICore(cont, mainElements, 1);
}
