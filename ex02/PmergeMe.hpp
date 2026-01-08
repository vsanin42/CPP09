/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PmergeMe.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vsanin <vsanin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/11 18:56:42 by vsanin            #+#    #+#             */
/*   Updated: 2026/01/08 15:54:06 by vsanin           ###   ########.fr       */
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
		std::vector<int> sortPairs(size_t compFactor, size_t pairNum);
	public:
		PmergeMe();
		PmergeMe(const PmergeMe& ref);
		PmergeMe& operator=(const PmergeMe& ref);
		~PmergeMe();

		void fillContainers(int argc, char** argv);
		void initMainPend(std::vector<int>& main, std::vector<int>& pend, size_t compFactor, size_t pairNum);
		void printContainers(PrintWhenOptions whenOption, PrintWhichOptions whichOption);
		void FJMI(size_t level);
};

template <typename T>
void printContainer(const T& cont, const std::string& name, int tabCount)
{
	std::cout << name << ":";
	for (int i = 0; i < tabCount; i++)
		std::cout << "\t";
	for (typename T::const_iterator it = cont.begin(); it != cont.end(); ++it)
		std::cout << " " << *it;
	std::cout << "\n";
}

#endif
