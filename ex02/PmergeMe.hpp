/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PmergeMe.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vsanin <vsanin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/11 18:56:42 by vsanin            #+#    #+#             */
/*   Updated: 2026/01/08 11:30:22 by vsanin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PMERGEME_HPP
#define PMERGEME_HPP

#define DEBUG 1
#include <vector>
#include <deque>
#include <cstddef>

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
