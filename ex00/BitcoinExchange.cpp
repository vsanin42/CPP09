/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   BitcoinExchange.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vsanin <vsanin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/16 14:50:20 by vsanin            #+#    #+#             */
/*   Updated: 2025/09/29 14:29:13 by vsanin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "BitcoinExchange.hpp"
#include <cstdlib>

BitcoinExchange::BitcoinExchange() : data() {}

BitcoinExchange::BitcoinExchange(std::ifstream& db)
{
	std::string line;
	db >> line;
	while (db >> line)
	{
		data[line.substr(0, 10)] = std::atof(line.substr(11, line.size() - 11).c_str());
	}
}

BitcoinExchange::BitcoinExchange(const BitcoinExchange& ref) : data(ref.data) {}

BitcoinExchange& BitcoinExchange::operator=(const BitcoinExchange& ref)
{
	if (this != &ref)
	{
		this->data = ref.data;
	}
	return *this;
}

BitcoinExchange::~BitcoinExchange() {}

// std::map<std::string, float>& BitcoinExchange::getInput(void)
// {
// 	return input;
// }

std::map<std::string, float>& BitcoinExchange::getData(void)
{
	return data;
}
