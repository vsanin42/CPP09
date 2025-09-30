/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vsanin <vsanin@student.42prague.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/11 18:57:53 by vsanin            #+#    #+#             */
/*   Updated: 2025/09/30 15:57:58 by vsanin           ###   ########.fr       */
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

static bool err(std::string msg, bool ret)
{
    std::cout << msg << "\n";
    return ret;
}

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

static bool validateInputLine(std::string& line, int& year, int& month, int& day, float& value)
{
    std::cout << line;
    int size = line.size();
    int n = 0;
    int fields = std::sscanf(line.c_str(), "%d-%d-%d | %f%n", &year, &month, &day, &value, &n); // %u for unsigned?

    if (n < size && fields == 4)
        return err(" ===> Error: bad input - extra character(s) found after value", false);
    if (n == 0 || fields < 4)
        return err(" ===> Error: bad input - left unfilled fields in [Y/M/D | V] format", false);
    if (month < 1 || month > 12)
        return err(" ===> Error: bad input - month out of range", false);
    if (year < 2009)
        return err(" ===> Error: bad input - bitcoin didn't exist back then/out of range", false);
    if (year > 2025)
        return err(" ===> Error: bad input - can't tell you future bitcoin value, sadly", false);
    
    int leapYearFlag = 0;
    int leapYears[] = {2012, 2016, 2020, 2024};
    for (int i = 0; i < 4; i++)
    {
        if (year == leapYears[i])
            leapYearFlag = 1;
        if (leapYearFlag && month == 2 && (day < 1 || day > 29))
            return err(" ===> Error: bad input - day out of range in February in a leap year", false);
    }
    if (!leapYearFlag && month == 2 && (day < 1 || day > 28))
        return err(" ===> Error: bad input - day out of range in February in a regular year", false);

    int longMonths[] = {1, 3, 5, 7, 8, 10, 12};
    int shortMonths[] = {4, 6, 9, 11};
    for (int i = 0; i < 7; i++)
    {
        if (month == longMonths[i] && (day < 1 || day > 31))
            return err(" ===> Error: bad input - day out of range in a long month", false);
    }
    for (int i = 0; i < 4; i++)
    {
        if (month == shortMonths[i] && (day < 1 || day > 30))
            return err(" ===> Error: bad input - day out of range in a short month", false);
    }

    if (value < 0 || value > 1000)
        return err(" ===> Error: bad input - value out of range", false);
    return true;
}

static bool validateDataLine(std::string& line, int& year, int& month, int& day, float& value)
{
    std::cout << line;
    int size = line.size();
    int n = 0;
    int fields = std::sscanf(line.c_str(), "%d-%d-%d | %f%n", &year, &month, &day, &value, &n); // %u for unsigned?

    if (n < size && fields == 4)
        return err(" ===> Error: bad input - extra character(s) found after value", false);
    if (n == 0 || fields < 4)
        return err(" ===> Error: bad input - left unfilled fields in [Y/M/D | V] format", false);
    if (month < 1 || month > 12)
        return err(" ===> Error: bad input - month out of range", false);
    if (year < 2009)
        return err(" ===> Error: bad input - bitcoin didn't exist back then/out of range", false);
    if (year > 2025)
        return err(" ===> Error: bad input - can't tell you future bitcoin value, sadly", false);
    
    int leapYearFlag = 0;
    int leapYears[] = {2012, 2016, 2020, 2024};
    for (int i = 0; i < 4; i++)
    {
        if (year == leapYears[i])
            leapYearFlag = 1;
        if (leapYearFlag && month == 2 && (day < 1 || day > 29))
            return err(" ===> Error: bad input - day out of range in February in a leap year", false);
    }
    if (!leapYearFlag && month == 2 && (day < 1 || day > 28))
        return err(" ===> Error: bad input - day out of range in February in a regular year", false);

    int longMonths[] = {1, 3, 5, 7, 8, 10, 12};
    int shortMonths[] = {4, 6, 9, 11};
    for (int i = 0; i < 7; i++)
    {
        if (month == longMonths[i] && (day < 1 || day > 31))
            return err(" ===> Error: bad input - day out of range in a long month", false);
    }
    for (int i = 0; i < 4; i++)
    {
        if (month == shortMonths[i] && (day < 1 || day > 30))
            return err(" ===> Error: bad input - day out of range in a short month", false);
    }

    if (value < 0 || value > 1000)
        return err(" ===> Error: bad input - value out of range", false);
    return true;
}

void displayBTC(std::ifstream& input, std::map<std::string, float> &data)
{
	std::string line;
	std::getline(input, line);
	if (line != "date | value")
	{
		std::cout << "Error: wrong input header formatting" << std::endl;
		return;
	}
	int year, month, day;
    float value;
	while (std::getline(input, line))
	{
        if (!validateInputLine(line, year, month, day, value))
            continue;
        // display
        // strftime() for easier comparison of two tm structs perhaps?
        std::cout << std::endl;
	}
	(void)data;
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
	BitcoinExchange exchange;


    std::map<std::string, float> data;
    std::string line;
    std::getline(db, line);
	if (line != "date | value")
	{
		std::cout << "Error: wrong input header formatting" << std::endl;
		return;
	}
	while (std::getline(db, line))
	{
        if (1/* !validateDataLine() */)
        {
            std::cout << "Error: [data.csv] cannot provide valid data\n";
            return 1;
        }
		data[line.substr(0, 10)] = std::atof(line.substr(11, line.size() - 11).c_str());
	}


    
    // exchange.setData();
	displayBTC(input, exchange.getData());
	return 0;
}
