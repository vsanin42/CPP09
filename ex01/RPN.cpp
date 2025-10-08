/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RPN.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vsanin <vsanin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/16 14:50:20 by vsanin            #+#    #+#             */
/*   Updated: 2025/10/08 13:57:41 by vsanin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "RPN.hpp"
#include <string>
#include <iostream>
#include <cstdlib>

/*================================[ OCF ]=====================================*/

RPN::RPN() : stack() {}

RPN::RPN(const RPN& ref) : stack(ref.stack) {}

RPN& RPN::operator=(const RPN& ref)
{
	if (this != &ref)
	{
		stack = ref.stack;
	}
	return *this;
}

RPN::~RPN() {}

/*================================[ UTILS ]====================================*/

static bool isOperator(char c)
{
	return c == '+' || c == '-' ||  c == '*' || c == '/';
}

void RPN::printStack(void)
{
	std::stack<double> copy = stack;
	std::cout << "stack: [ ";
	while (!copy.empty())
	{
		std::cout << copy.top() << " ";
		copy.pop();
	}
	std::cout << "]\n";
}

/*================================[ RPN ]=====================================*/

void RPN::applyOperator(char c)
{
	if (stack.size() < 2)
		throw std::runtime_error("Error: incorrect RPN - insufficient stack size for performing leftover operations");
	if (c == '+') std::cout << "operator: + | ";
	if (c == '-') std::cout << "operator: - | ";
	if (c == '*') std::cout << "operator: * | ";
	if (c == '/') std::cout << "operator: / | ";

	double rightOperand = stack.top();
	stack.pop();
	double leftOperand = stack.top();
	stack.pop();

	if (c == '/' && rightOperand == 0)
		throw std::runtime_error("Error: diving by zero");
	double result = 0;
	if (c == '+') result = leftOperand + rightOperand;
	if (c == '-') result = leftOperand - rightOperand;
	if (c == '*') result = leftOperand * rightOperand;
	if (c == '/') result = leftOperand / rightOperand;
	std::cout << "result: " << result << "\n";
	stack.push(result);
	printStack();
}

void RPN::solveRPN(std::string& arg, size_t len)
{
	size_t maxStackSize = 0;
	size_t operatorCount = 0;
	for (size_t i = 0; i < len; i++)
	{
		if (arg[i] == ' ')
			continue;
		if (isdigit(arg[i]) && i + 1 != len && isdigit(arg[i + 1]))
			throw std::runtime_error("Error: out of range [0, 9] number detected");
		if (isOperator(arg[i]))
		{
			operatorCount++;
			applyOperator(arg[i]);
		}
		else
		{
			maxStackSize++;
			double num = std::atof(arg.substr(i, 1).c_str());
			stack.push(num);
			printStack();
		}
	}
	if (operatorCount != maxStackSize - 1)
		throw std::runtime_error("Error: incorrect RPN - final value wasn't reached, extra elements left in the stack");
	else
		std::cout << "final result: " << stack.top() << "\n";
}
