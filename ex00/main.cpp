/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vsanin <vsanin@student.42prague.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/11 18:57:53 by vsanin            #+#    #+#             */
/*   Updated: 2025/09/30 11:39:26 by vsanin           ###   ########.fr       */
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

void displayBTC(std::ifstream& input, std::map<std::string, float> &data)
{
	std::string line;
	std::getline(input, line);
    std::cout << line << std::endl;
	if (line != "date | value")
	{
		std::cout << "Error: wrong input header formatting." << std::endl;
		return;
	}
	int year, month, day, n;
    float value;
	while (std::getline(input, line))
	{
		// 5. keep date as string, will be checked against .csv.
		// 6. check the input after separator - has to be int or float in range 0-1000. manual or stringstream.
		// 7. produce the output.
        
		// todo?: similar checks for .csv just in case but debatable.

        std::cout << line;
        int size = line.size();
        n = 0;
		int fields = std::sscanf(line.c_str(), "%d-%d-%d | %f%n", &year, &month, &day, &value, &n); // %u for unsigned?
        if (n < size && fields == 4)
        {
            std::cout << " ===> Error: bad input - extra character(s) found after value." << std::endl;
            continue;
        }
        if (n == 0 || fields < 4)
        {
            std::cout << " ===> Error: bad input - filled " << fields << "/4 fields." << std::endl;
            continue;
        }
        if (month < 1 || month > 12)
        {
            std::cout << " ===> Error: bad input - month out of range" << std::endl;
            continue;
        }
        if (year < 2009)
        {
            std::cout << " ===> Error: bad input - bitcoin didn't exist back then." << std::endl;
            continue;
        }
        if (year > 2025)
        {
            std::cout << " ===> Error: bad input - can't tell you future bitcoin value, sadly." << std::endl;
            continue;
        }
        int failFlag = 0;
        int leapYearFlag = 0;
        int leapYears[] = {2012, 2016, 2020, 2024};
        for (int i = 0; i < 4; i++)
        {
            if (year == leapYears[i])
                leapYearFlag = 1;
            if (leapYearFlag && month == 2 && (day < 1 || day > 29))
            {
                std::cout << " ===> Error: bad input - day out of range in February in a leap year" << std::endl;
                failFlag = 1;
                break;
            }
        }
        if (failFlag)
            continue;
        if (!leapYearFlag && month == 2 && (day < 1 || day > 28))
        {
            std::cout << " ===> Error: bad input - day out of range in February in a regular year" << std::endl;
            continue;
        }
        int longMonths[] = {1, 3, 5, 7, 8, 10, 12};
        int shortMonths[] = {4, 6, 9, 11};
        for (int i = 0; i < 7; i++)
        {
            if (month == longMonths[i] && (day < 1 || day > 31))
            {
                std::cout << " ===> Error: bad input - day out of range in a long month" << std::endl;
                failFlag = 1;
                break;
            }
        }
        if (failFlag)
            continue;
        for (int i = 0; i < 4; i++)
        {
            if (month == shortMonths[i] && (day < 1 || day > 30))
            {
                std::cout << " ===> Error: bad input - day out of range in a short month" << std::endl;
                failFlag = 1;
                continue;
            }
        }
        if (failFlag)
            continue;
        if (value < 0 || value > 1000)
        {
            std::cout << " ===> Error: bad input - value out of range" << std::endl;
            continue;
        }
        std::cout << std::endl;
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
	if (!validateFiles(input, db))
		return 1;
	BitcoinExchange exchange(db);
	displayBTC(input, exchange.getData());
	return 0;
}
