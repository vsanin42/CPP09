/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PmergeMe.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vsanin <vsanin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/11 18:56:42 by vsanin            #+#    #+#             */
/*   Updated: 2026/01/19 18:51:35 by vsanin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PMERGEME_HPP
#define PMERGEME_HPP

#define DEBUG 1

#define  RESET   "\033[0m"

#define  RED      "\033[31m"
#define  GREEN    "\033[32m"
#define  YELLOW   "\033[33m"
#define  BLUE     "\033[34m"
#define  MAGENTA  "\033[35m"
#define  CYAN     "\033[36m"
#define  WHITE    "\033[37m"
#define  ORANGE   "\033[38;5;208m"

#define  BRED     "\033[91m"
#define  BGREEN   "\033[92m"
#define  BYELLOW  "\033[93m"
#define  BBLUE    "\033[94m"
#define  BMAGENTA "\033[95m"
#define  BCYAN    "\033[96m"

#define  BOLD     "\033[1m"
#define  DIM      "\033[2m"

#include <vector>
#include <deque>
#include <cstddef>
#include <string>
#include <iostream>
#include <cmath>

enum PrintWhenOptions { BEFORE, AFTER, NONE };
enum PrintWhichOptions { VECTOR, DEQUE, BOTH };

class PmergeMe
{
	private:
		std::vector<int> vec;
		std::deque<int> deq;
		size_t size;
		size_t comparisons;
		static int validateArg(const char* argv);
		static size_t jacobstahlGenerator(size_t level);
		static void jacobstahlCreate(std::vector<size_t>& jacobstahlSequence, size_t pendElementsCount);
		
		template <typename T>
		T sortPairs(T& cont, size_t compFactor, size_t pairNum);
	public:
		PmergeMe();
		PmergeMe(const PmergeMe& ref);
		PmergeMe& operator=(const PmergeMe& ref);
		~PmergeMe();

		void fillContainers(int argc, char** argv);
		std::vector<int>& getVector(void);
		std::deque<int>& getDeque(void);
		void setComparisons(size_t count);
		template <typename T>
		void initMainPend(T& cont, T& main, T& pend, size_t compFactor, size_t pairNum);

		template <typename T, typename U>
		void structureMain(U& mainElements, T& main, size_t compFactor);
		template <typename T, typename U>
		void structurePend(U& pendElements, T& pend, size_t compFactor);
		
		void printContainers(PrintWhenOptions whenOption, PrintWhichOptions whichOption);
		
		template <typename U>
		void printMain(U& mainElements);
		template <typename U>
		void printPend(U& pendElements);
		
		template <typename U>
		size_t binarySearch(U& mainElements, U& pendElements, size_t startIndex);
		
		void FJMIEntry(std::vector<int>& cont);
		void FJMIEntry(std::deque<int>& cont);
		template <typename T, typename U>
		void FJMICore(T& cont, U& mainElements, size_t level);
};


/*============================================================================*/
/*||                               PRINT                                    ||*/
/*============================================================================*/

template <typename T>
void printContainer(const T& cont, const std::string& name, int tabCount, bool printNewline)
{
	if (name.size())
		std::cout << name << ":";
	for (int i = 0; i < tabCount; i++)
		std::cout << "\t";
	for (typename T::const_iterator it = cont.begin(); it != cont.end(); ++it)
		std::cout << " " << *it;
	if (printNewline)
		std::cout << "\n";
}

/*============================================================================*/
/*||                               PAIRS                                    ||*/
/*============================================================================*/

template <typename T>
T PmergeMe::sortPairs(T& cont, size_t compFactor, size_t pairNum)
{
	for (size_t i = compFactor * 2 - 1; i < size; i += compFactor * 2)
	{
		if (DEBUG) std::cout << "Checking - Pair: " << cont[i - compFactor] << " " << cont[i];
		if (cont[i - compFactor] > cont[i])
		{
			if (DEBUG) std::cout << " - " << RED << "Needs swapping." << RESET;

			size_t j = i - compFactor;
			size_t k = i;
			int temp = 0;
			while (k > i - compFactor)
			{
				temp = cont[j];
				cont[j] = cont[k];
				cont[k] = temp;
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
		T remainder(cont.begin() + (compFactor * (2 * pairNum)), cont.end());
		if (DEBUG)
		{
			std::cout << MAGENTA;
			printContainer(remainder, "Remainder", 1, 1);
			std::cout << RESET;
		}
		return remainder;
	}
	
	return T();
}

/*============================================================================*/
/*||                            MAIN & PEND                                 ||*/
/*============================================================================*/

template <typename T>
void PmergeMe::initMainPend(T& cont, T& main, T& pend, size_t compFactor, size_t pairNum)
{
	if (pairNum > 1)
	{
		size_t i = compFactor * 2;
		for (size_t count = 1; count <= (size / compFactor) - 2; count++)
		{
			for (size_t j = 0; j < compFactor; j++)
			{
				count % 2 == 0 ? main.push_back(cont[i]) : pend.push_back(cont[i]);
				i++;
			}
		}
	}
}

template <typename T, typename U>
void PmergeMe::structureMain(U& mainElements, T& main, size_t compFactor)
{
	mainElements.clear();
	size_t j = 0;
	for (size_t i = 0; i < main.size(); i += compFactor)
		mainElements.push_back(std::make_pair(T(main.begin() + i, main.begin() + i + compFactor), j++));
	if (DEBUG) printMain(mainElements);
}

template <typename T, typename U>
void PmergeMe::structurePend(U& pendElements, T& pend, size_t compFactor)
{
	size_t k = 2;
	for (size_t i = 0; i < pend.size(); i += compFactor)
		pendElements.push_back(std::make_pair(T(pend.begin() + i, pend.begin() + i + compFactor), k++));
	if (DEBUG) printPend(pendElements);
}

template <typename U>
void PmergeMe::printMain(U& mainElements)
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

template <typename U>
void PmergeMe::printPend(U& pendElements)
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

/*============================================================================*/
/*||                           BINARY SEARCH                                ||*/
/*============================================================================*/

template <typename U>
size_t PmergeMe::binarySearch(U& mainElements, U& pendElements, size_t startIndex)
{
	size_t rightmost = mainElements[0].first.size() - 1; // compFactor - 1
	size_t upperBound = mainElements.size() - 1; // bad output below
	if (DEBUG)
	{
		std::cout << BOLD BMAGENTA << "Binary search" << RESET << "\n";
		std::cout << MAGENTA << "Defining search range" << RESET << "\n";
		printContainer(pendElements[startIndex].first, "Trying to insert element", 0, 1);
		std::cout << "Currently searching main up to max element: " << BGREEN << "[" << mainElements[upperBound].second << "]\n" << RESET;
	}
	for (size_t i = 0; i < mainElements.size(); i++)
	{
		if (mainElements[i].second == pendElements[startIndex].second)
		{
			if (DEBUG) std::cout << "Found a corresponding index in main. The pend element being inserted will be smaller that that, search field will shrink\n";
			upperBound = i - 1;
		}
	}
	
	if (DEBUG)
	{
		std::cout << "After adjustment - Searching up to element: " << BGREEN << "[" << mainElements[upperBound].second << "]\n" << RESET;
		std::cout << MAGENTA << "\nFinding an index for insertion\n" << RESET;
	}
	size_t lowerBound = 0;
	size_t mid = 0;
	
	// todo optimization
	while (lowerBound != upperBound)
	{
		mid = lowerBound + ((upperBound - lowerBound) / 2);

		if (DEBUG)
		{
			std::cout << "In range: " << ORANGE << lowerBound << RESET " - " << ORANGE << upperBound << RESET << " \t|\t";
			std::cout << "Step 1: finding mid index: " ORANGE << mid << RESET << "\t|\t";
			std::cout << "Step 2: comparing numbers: " << BCYAN << pendElements[startIndex].first[rightmost] << RESET BOLD << " ? " << RESET BCYAN << mainElements[mid].first[rightmost] << RESET << "\n";
		}
		if (pendElements[startIndex].first[rightmost] >= mainElements[mid].first[rightmost])
		{
			if (DEBUG) std::cout << "Updated " << BOLD << "lowerBound" << RESET << " - shifted up\n";
			lowerBound = mid + 1;
		}
		else
		{
			if (DEBUG) std::cout << "Updated " << BOLD << "upperBound" << RESET << " - shifted down\n";
			upperBound = mid ? mid - 1 : 0;
		}
		comparisons++;
	}
	if (DEBUG)
	{
		std::cout << BOLD << "lowerBound == upperBound" << RESET << " - potential index for insertion found: " << ORANGE << lowerBound << RESET << "\n";
		std::cout << "Comparing number to insert (" << BCYAN << pendElements[startIndex].first[rightmost] << RESET
				  << ") to current number at the index (" << BCYAN << mainElements[lowerBound].first[rightmost] << RESET << ")\n";
	}
	if (pendElements[startIndex].first[rightmost] > mainElements[lowerBound].first[rightmost])
	{
		if (DEBUG) std::cout << "Updating index (++) / swapping elements\n";
		lowerBound++;
		comparisons++;
	}
	else
	{
		if (DEBUG) std::cout << "Index unchanged after final comparison\n";
		comparisons++;
	}
	return lowerBound;
}

/*============================================================================*/
/*||                               FJMI                                     ||*/
/*============================================================================*/

template <typename T, typename U>
void PmergeMe::FJMICore(T& cont, U& mainElements, size_t level)
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
	
	if (DEBUG) printContainer(cont, "Container before pair sorting", 1, 1);
	T remainder = sortPairs(cont, compFactor, pairNum);
	if (DEBUG)
	{
		std::cout << GREEN;
		printContainer(cont, "Container after pair sorting", 1, 1);
		std::cout << RESET;
	}
	

	
	// 3. Recursion logic - all sorting in the previous step must happen first, then insertion on unwind.
	FJMICore(cont, mainElements, level + 1);


	
	// 4. After recursion returns, insertion.
	// 4.1. Initialize raw main and pend.
	T main(cont.begin(), cont.begin() + (compFactor * 2));
	T pend;

	initMainPend(cont, main, pend, compFactor, pairNum);

	if (DEBUG)
	{
		std::cout << BOLD YELLOW << "Level: " << BCYAN << level << RESET
				  << BOLD YELLOW << ", compFactor: " << BCYAN << compFactor << RESET
				  << BOLD YELLOW << ", pairNum: " << BCYAN << pairNum << "\n" << RESET;
		printContainer(cont, "Container", 2, 1);
		printContainer(main, "Raw Main (b1 & a's)", 1, 1);
		printContainer(pend, "Raw Pend (b's from b2)", 1, 1);
		std::cout << "\n";
	}


	
	// 4.2. Create a Jacobstahl sequence for the current level and pend size.
	size_t pendElementsCount = pend.size() / compFactor;
	std::vector<size_t> jacobstahlSequence;
	jacobstahlCreate(jacobstahlSequence, pendElementsCount);


	
	// 4.3. Structure raw main and pend to contain predefined indexes to track during the insertion.
	structureMain(mainElements, main, compFactor);
	
	if (!pendElementsCount)
	{
		std::cout << YELLOW << "\nPend empty, nothing to insert.\n" << RESET;
		std::cout << "\n----------------------------------------\n\n";
		return;
	}
	
	U pendElements;
	structurePend(pendElements, pend, compFactor);

	if (DEBUG) std::cout << "\n---\n";

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

		if (DEBUG)
		{
			std::cout << BBLUE << "\nCurrent selected Jacobstahl number: " << BOLD << J << RESET << "\n";
		}
		if (J > 0)
		{
			for (size_t i = 0; i < pendElements.size(); i++)
			{
				if (pendElements[i].second == J)
				{
					startIndex = i;
					if (DEBUG) std::cout << "Starting inserting with pend element " << BGREEN << "[" << pendElements[i].second << "]" << RESET
										 << ", index matches " << BBLUE "J\n" << RESET;
					// if (DEBUG) printContainer(pendElements[i].first, "Element", 0, 1);
				}
			}
		}
		else
		{
			if (DEBUG) std::cout << YELLOW "Ran out of Jacobstahl numbers. Inserting the remaining pend elements in reverse order\n" << RESET;
			startIndex = pendElements.size() - 1; // correct?
		}
		
		size_t insertionIndex = binarySearch(mainElements, pendElements, startIndex);
		mainElements.insert(mainElements.begin() + insertionIndex, pendElements[startIndex]);
		pendElements.erase(pendElements.begin() + startIndex);
		if (DEBUG) printMain(mainElements);
		if (DEBUG) printPend(pendElements);
		startIndex--;
		insertionsCount++;
		std::cout << "\n---\n";
	}

	if (DEBUG) printContainer(cont, "\nOriginal container before insertion at this level", 1, 1);
	for (size_t el = 0; el < mainElements.size(); el++)
	{
		for (size_t i = 0; i < compFactor; i++)
		{
			cont[i + el * compFactor] = mainElements[el].first[i];
		}
	}
	if (DEBUG) printContainer(cont, "Original container after insertion at this level", 1, 1);

	std::cout << "\nComparisons: " << comparisons << "\n";
	std::cout << "\n----------------------------------------\n\n";
}

#endif
