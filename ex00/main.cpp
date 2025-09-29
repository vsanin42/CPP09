/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vsanin <vsanin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/11 18:57:53 by vsanin            #+#    #+#             */
/*   Updated: 2025/09/29 15:14:45 by vsanin           ###   ########.fr       */
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

static bool validate_files(std::ifstream& input, std::ifstream& db)
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

void displayBTC(std::ifstream& input, std::map<std::string, float> &data)
{
	std::string line;
	std::getline(input, line);
	if (line != "date | value")
	{
		std::cout << "Error: wrong input header formatting." << std::endl;
		return;
	}
	int year, month, day;
	while (std::getline(input, line))
	{
		/* 
		1. find separator. if not found -> bad input
		2. count length to separator. if not 11 -> bad input
		3. substr from first 10/11 chars. it has to be the date.
		4. verify the date, possibly with sscanf. simple char by char also possible.
		5. keep date as string, will be checked against .csv.
		6. check the input after separator - has to be int or float in range 0-1000. manual or stringstream.
		7. produce the output.

		// todo?: similar checks for .csv just in case but debatable.
		*/
		std::string date = line.substr(0, 11);
		int ret = std::sscanf(date.c_str(), "%d-%d-%d", &year, &month, &day);
		
	}
	(void)data;
}

int main(int argc, char** argv)
{
	if (argc != 2)
	{
		std::cerr << "Error: one argument expected." << std::endl;
		return 1;
	}
	std::ifstream input(argv[1]);
	std::ifstream db("data.csv");
	if (!validate_files(input, db))
		return 1;
	BitcoinExchange exchange(db);
	displayBTC(input, exchange.getData());
	return 0;
}
