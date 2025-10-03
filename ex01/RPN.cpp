/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RPN.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vsanin <vsanin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/16 14:50:20 by vsanin            #+#    #+#             */
/*   Updated: 2025/10/03 17:45:53 by vsanin           ###   ########.fr       */
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

std::stack<double>& RPN::getStack(void) { return stack; }

/*================================[ UTILS ]====================================*/

static bool isOperator(char c)
{
	return c == '+' || c == '-' ||  c == '*' || c == '/';
}

void RPN::printStack(std::stack<double> stack)
{
	std::cout << "stack: [ ";
	while (!stack.empty())
	{
		std::cout << stack.top() << " ";
		stack.pop();
	}
	std::cout << "]\n";
}

/*================================[ RPN ]=====================================*/

static void applyOperator(RPN& rpn, char c)
{
	if (rpn.getStack().size() < 2)
		throw std::runtime_error("Error: incorrect RPN - insufficient stack size for performing leftover operations");
	if (c == '+') std::cout << "operator: + | ";
	if (c == '-') std::cout << "operator: - | ";
	if (c == '*') std::cout << "operator: * | ";
	if (c == '/') std::cout << "operator: / | ";

	double rightOperand = rpn.getStack().top();
	rpn.getStack().pop();
	double leftOperand = rpn.getStack().top();
	rpn.getStack().pop();

	double result = 0;
	if (c == '+') result = leftOperand + rightOperand;
	if (c == '-') result = leftOperand - rightOperand;
	if (c == '*') result = leftOperand * rightOperand;
	if (c == '/') result = leftOperand / rightOperand;
	std::cout << "result: " << result << "\n";
	rpn.getStack().push(result);
	RPN::printStack(rpn.getStack());
}

void RPN::solveRPN(RPN& rpn, std::string& arg, size_t len)
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
			applyOperator(rpn, arg[i]);
		}
		else
		{
			maxStackSize++;
			double num = std::atof(arg.substr(i, 1).c_str());
			rpn.getStack().push(num);
			RPN::printStack(rpn.getStack());
		}
	}
	if (operatorCount != maxStackSize - 1)
		throw std::runtime_error("Error: incorrect RPN - final value wasn't reached, extra elements left in the stack");
	else
		std::cout << "final result: " << rpn.getStack().top() << "\n";
}
