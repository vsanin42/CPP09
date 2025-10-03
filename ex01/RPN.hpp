/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RPN.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vsanin <vsanin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/11 18:56:42 by vsanin            #+#    #+#             */
/*   Updated: 2025/10/03 18:15:05 by vsanin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef RPN_HPP
#define RPN_HPP

#include <stack>
#include <string>

class RPN
{
	private:
		std::stack<double> stack;
		void applyOperator(char c);
		void printStack(void);
	public:
		RPN();
		RPN(const RPN& ref);
		RPN& operator=(const RPN& ref);
		~RPN();

		void solveRPN(std::string& arg, size_t len);
};

#endif
