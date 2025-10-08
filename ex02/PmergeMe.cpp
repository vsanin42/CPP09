/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PmergeMe.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vsanin <vsanin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/16 14:50:20 by vsanin            #+#    #+#             */
/*   Updated: 2025/10/08 15:12:40 by vsanin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "PmergeMe.hpp"

/*================================[ OCF ]=====================================*/

PmergeMe::PmergeMe() : vector() {}

PmergeMe::PmergeMe(const PmergeMe& ref) : vector(ref.vector) {}

PmergeMe& PmergeMe::operator=(const PmergeMe& ref)
{
	if (this != &ref)
	{
		vector = ref.vector;	
	}
	return *this;
}

PmergeMe::~PmergeMe() {}
