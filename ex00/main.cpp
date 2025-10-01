/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vsanin <vsanin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/11 18:57:53 by vsanin            #+#    #+#             */
/*   Updated: 2025/10/01 20:32:19 by vsanin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "BitcoinExchange.hpp"
#include <iostream>
#include <fstream>
#include <map>
#include <string>
#include <cstdlib>
#include <iomanip>
#include <ctime>
#include <cstdio>

static bool validateFiles(std::ifstream& input, std::ifstream& db)
{
	if (!input.is_open())
	{
		std::cerr << "Error: could not open input file." << std::endl;
		return false;
	}
	if (!db.is_open())
	{
		std::cerr << "Error: could not open database file." << std::endl;
		input.close();
		return false;
	}
	return true;
}

int main(int argc, char** argv)
{
	if (argc != 2)
	{
		std::cerr << "Error: one argument expected" << std::endl;
		return 1;
	}
	std::ifstream input(argv[1]);
	std::ifstream db("data.csv");
	if (!validateFiles(input, db))
		return 1;

    std::map<myDate, float> data;
    if (!BitcoinExchange::validateData(db, data))
		return 1;

	BitcoinExchange exchange(data);
	if (!exchange.displayBTC(input, data))
		return 1;
	return 0;
}
