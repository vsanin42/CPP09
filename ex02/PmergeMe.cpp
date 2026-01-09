/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PmergeMe.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vsanin <vsanin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/16 14:50:20 by vsanin            #+#    #+#             */
/*   Updated: 2026/01/09 18:06:14 by vsanin           ###   ########.fr       */
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

void PmergeMe::printMain(std::vector< std::pair<std::vector<int>, size_t> >& mainElements)
{
	std::cout << "Main:" << BGREEN << " |" << RESET;
	for (size_t i = 0; i < mainElements.size(); i++)
	{
		std::cout << BGREEN << " [" << mainElements[i].second << "]" << RESET;
		printContainer(mainElements[i].first, "", 0, 0);
		std::cout << BGREEN << " |" << RESET;
	}
	std::cout << "\n";
}

void PmergeMe::printPend(std::vector< std::pair<std::vector<int>, size_t> >& pendElements)
{
	std::cout << "Pend:" << BGREEN << " |" << RESET;
	for (size_t i = 0; i < pendElements.size(); i++)
	{
		std::cout << BGREEN << " [" << pendElements[i].second << "]" << RESET;
		printContainer(pendElements[i].first, "", 0, 0);
		std::cout << BGREEN << " |" << RESET;
	}
	std::cout << "\n";
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
	if (DEBUG) std::cout << "\nIs there a remainder after size % compFactor? : " << (isOdd ? GREEN : RED) << (isOdd ? "YES" : "NO") << "\n" << RESET;
	if (isOdd)
	{
		std::vector<int> remainder(vec.begin() + (compFactor * (2 * pairNum)), vec.end());
		if (DEBUG)
		{
			std::cout << MAGENTA;
			printContainer(remainder, "Remainder", 1, 1);
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

void PmergeMe::structureMain(std::vector< std::pair<std::vector<int>, size_t> >& mainElements, std::vector<int> main, size_t compFactor)
{
	size_t j = 0;
	for (size_t i = 0; i < main.size(); i += compFactor)
		mainElements.push_back(std::make_pair(std::vector<int>(main.begin() + i, main.begin() + i + compFactor), j++));
	if (DEBUG) printMain(mainElements);
}

void PmergeMe::structurePend(std::vector< std::pair<std::vector<int>, size_t> >& pendElements, std::vector<int> pend, size_t compFactor)
{
	size_t k = 2;
	for (size_t i = 0; i < pend.size(); i += compFactor)
		pendElements.push_back(std::make_pair(std::vector<int>(pend.begin() + i, pend.begin() + i + compFactor), k++));
	if (DEBUG) printPend(pendElements);
}

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

size_t PmergeMe::binarySearch(std::vector< std::pair<std::vector<int>, size_t> >& mainElements, std::vector< std::pair<std::vector<int>, size_t> >& pendElements, size_t startIndex)
{
	size_t rightmost = mainElements[0].first.size() - 1; // compFactor - 1
	size_t upperBound = mainElements.size() - 1; // bad output below
	printContainer(pendElements[startIndex].first, "Trying to insert element", 0, 1);
	std::cout << "Defining search range. Currently searching main up to index " << upperBound << " (literal) or " << mainElements[upperBound].second << " (displayed)\n";
	for (size_t i = 0; i < mainElements.size(); i++)
	{
		if (mainElements[i].second == pendElements[startIndex].second)
		{
			std::cout << "Found a corresponding index in main\n";
			std::cout << "The pend element being inserted will be smaller that that, search field will shrink\n";
			upperBound = i - 1;
		}
	}
	std::cout << "After index lookup: Currently searching main up to index " << upperBound << " (literal) or " << mainElements[upperBound].second << " (displayed)\n";
	
	std::cout << "Binary search for a place to insert\n";

	size_t lowerBound = 0;
	size_t mid = 0;
	
	while (lowerBound != upperBound)
	{
		mid = lowerBound + ((upperBound - lowerBound) / 2);

		std::cout << "In range: " << lowerBound << " - " << upperBound << "\n";
		std::cout << "Step 1: finding mid index: " << mid << "\n";
		std::cout << "Step 2: comparing: " << pendElements[startIndex].first[rightmost] << " ? " << mainElements[mid].first[rightmost] << "\n";

		// duplicates here?
		if (pendElements[startIndex].first[rightmost] > mainElements[mid].first[rightmost])
		{
			std::cout << "Updated lowerBound up\n";
			lowerBound = mid + 1;
			comparisons++;
		}
		else if (pendElements[startIndex].first[rightmost] < mainElements[mid].first[rightmost])
		{
			std::cout << "Updated upperBound down\n";
			upperBound = mid ? mid - 1 : 0;
			comparisons++;
		}
	}
	std::cout << "Step 3: lower == upper - potential index for insertion found: " << lowerBound << "\n";
	if (pendElements[startIndex].first[rightmost] > mainElements[lowerBound].first[rightmost]) // we either place it below or above current found index
	{
		std::cout << "Updated lowerBound - special case\n";
		lowerBound++;
		comparisons++;
	}
	return lowerBound;
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
			std::cout << BOLD YELLOW << "\nPairs cannnot be formed at this level (" << BCYAN << level << YELLOW << "). Returning.\n" << RESET;
			std::cout << "\n----------------------------------------\n";

			std::cout << BOLD << BBLUE
			<< "\n----------------------------------------"
			<< "\n|          STEP 2: Inserting           |"
			<< "\n----------------------------------------\n\n" << RESET;
		}
		return;
	}
	
	if (DEBUG)
	{
		if (level == 1)
			std::cout << BOLD << BBLUE
			<< "\n----------------------------------------"
			<< "\n|           STEP 1: Sorting            |"
			<< "\n----------------------------------------\n" << RESET;
		std::cout << "\n----------------------------------------\n\n";
		std::cout << BOLD YELLOW<< "Entering function call with LEVEL " << BCYAN << level << RESET << ".\n";
		std::cout << "At this level there will be " << BCYAN << pairNum << RESET << " pairs. ";
		std::cout << "A pair has " << BCYAN << numsInPair << RESET << " numbers. ";
		std::cout << "An element in a pair has " << BCYAN << numsInElement << RESET << " numbers. ";
		std::cout << "There will be " << BCYAN << remainingNums << RESET << " remaining numbers.\n\n";
	}


	
	// 2. Basic sorting.
	size_t compFactor = numsInElement;
	
	if (DEBUG) printContainers(BEFORE, VECTOR);
	std::vector<int> remainder = sortPairs(compFactor, pairNum);
	if (DEBUG)
	{
		std::cout << GREEN;
		printContainers(AFTER, VECTOR);
		std::cout << RESET;
	}
	

	
	// 3. Recursion logic - all sorting in the previous step must happen first, then insertion on unwind.
	FJMI(level + 1);


	
	// 4. After recursion returns, insertion.
	// 4.1. Initialize raw main and pend.
	std::vector<int> main(vec.begin(), vec.begin() + (compFactor * 2));
	std::vector<int> pend;

	initMainPend(main, pend, compFactor, pairNum);

	if (DEBUG)
	{
		std::cout << BOLD YELLOW << "Level: " << BCYAN << level << RESET
				  << BOLD YELLOW << ", compFactor: " << BCYAN << compFactor << RESET
				  << BOLD YELLOW << ", pairNum: " << BCYAN << pairNum << "\n" << RESET;
		printContainer(vec, "Vector", 3, 1);
		printContainer(main, "Raw Main (b1 & a's)", 1, 1);
		printContainer(pend, "Raw Pend (b's from b2)", 1, 1);
		std::cout << "\n";
	}


	
	// 4.2. Create a Jacobstahl sequence for the current level and pend size.
	size_t pendElementsCount = pend.size() / compFactor;
	std::vector<size_t> jacobstahlSequence;
	jacobstahlCreate(jacobstahlSequence, pendElementsCount);


	
	// 4.3. Structure raw main and pend to contain predefined indexes to track during the insertion.
	std::vector< std::pair<std::vector<int>, size_t> > mainElements;
	structureMain(mainElements, main, compFactor);
	
	if (!pendElementsCount)
	{
		std::cout << YELLOW << "\nPend empty, nothing to insert.\n" << RESET;
		std::cout << "\n----------------------------------------\n";
		return;
	}
	
	std::vector< std::pair<std::vector<int>, size_t> > pendElements;
	structurePend(pendElements, pend, compFactor);



	int n = 0; // increase when we want to shift to the next jacobstahl number in the sequence
	size_t insertionsCount = 0;
	size_t startIndex = 0;
	while (pendElements.size())
	{
		if ((n > 0 && insertionsCount == jacobstahlSequence[n] - jacobstahlSequence[n - 1]) || (n == 0 && insertionsCount == 2))
		{
			n++;
			insertionsCount = 0;
		}
			
		size_t J = 0;
		if (n >= static_cast<int>(jacobstahlSequence.size()) || n == -1)
		{
			n = -1;
			J = 0;
		}
		else
			J = jacobstahlSequence[n];
		// if (level < 2)
		// 	break;
		std::cout << "\nTesting here\n\n";
		std::cout << "Current selected Jacobstahl number: " << J << "\n";
		
		if (J > 0)
		{
			// startIndex = 0;
			for (size_t i = 0; i < pendElements.size(); i++)
			{
				if (pendElements[i].second == J)
				{
					startIndex = i;
					std::cout << "Starting inserting with pend element " << pendElements[i].second << ", index matches J\n";
					printContainer(pendElements[i].first, "Element", 0, 1);
				}
			}
		}
		else
		{
			std::cout << "Ran out of Jacobstahl numbers. Inserting the remaining pend elements in reverse order\n";
			startIndex = pendElements.size() - 1; // correct?
		}
		
		size_t insertionIndex = binarySearch(mainElements, pendElements, startIndex);
		mainElements.insert(mainElements.begin() + insertionIndex, pendElements[startIndex]);
		pendElements.erase(pendElements.begin() + startIndex);
		printMain(mainElements);
		printPend(pendElements);
		startIndex--;
		insertionsCount++;
	}

	printContainers(BEFORE, VECTOR);
	for (size_t el = 0; el < mainElements.size(); el++)
	{
		for (size_t i = 0; i < compFactor; i++)
		{
			vec[i + el * compFactor] = mainElements[el].first[i];
		}
	}
	printContainers(AFTER, VECTOR);

	std::cout << "\nComparisons: " << comparisons << "\n";
	std::cout << "\n----------------------------------------\n\n";
}
