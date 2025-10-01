/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vsanin <vsanin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/11 18:57:53 by vsanin            #+#    #+#             */
/*   Updated: 2025/10/01 16:53:15 by vsanin           ###   ########.fr       */
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

struct myDate{
	int year, month, day;
	bool operator<(const myDate& rhs) const
	{
		if (year < rhs.year) return true;
		if (month < rhs.month) return true;	
		if (day < rhs.day) return true;
		return false;
	};
	bool operator==(const myDate& rhs) const
	{
		if (year == rhs.year && month == rhs.month && day == rhs.day) return true;
		return false;
	};
};

static bool err(std::string msg, bool ret)
{
    std::cout << msg << "\n";
    return ret;
}

static void printHeader(const std::string& title) {
    const int width = 60;
    int side = (width - 2 - (int)title.size()) / 2;
    if (side < 0) side = 0;
    std::string pad(side, '=');
    std::cout << pad << ' ' << title << ' ' << pad;
    if (((int)title.size()) % 2 != (width % 2)) std::cout << '=';
    std::cout << "\n";
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

static bool performDateChecks(myDate date)
{
	if (date.month < 1 || date.month > 12)
        return err("❌ Error: bad input - month out of range", false);
    if (date.year < 2009)
        return err("❌ Error: bad input - bitcoin didn't exist back then/out of range", false);
    if (date.year > 2025)
        return err("❌ Error: bad input - can't tell you future bitcoin value, sadly", false);
    
    int leapYearFlag = 0;
    int leapYears[] = {2012, 2016, 2020, 2024};
    for (int i = 0; i < 4; i++)
    {
        if (date.year == leapYears[i])
            leapYearFlag = 1;
        if (leapYearFlag && date.month == 2 && (date.day < 1 || date.day > 29))
            return err("❌ Error: bad input - day out of range in February in a leap year", false);
    }
    if (!leapYearFlag && date.month == 2 && (date.day < 1 || date.day > 28))
        return err("❌ Error: bad input - day out of range in February in a regular year", false);

    int longMonths[] = {1, 3, 5, 7, 8, 10, 12};
    int shortMonths[] = {4, 6, 9, 11};
    for (int i = 0; i < 7; i++)
    {
        if (date.month == longMonths[i] && (date.day < 1 || date.day > 31))
            return err("❌ Error: bad input - day out of range in a long month", false);
    }
    for (int i = 0; i < 4; i++)
    {
        if (date.month == shortMonths[i] && (date.day < 1 || date.day > 30))
            return err("❌ Error: bad input - day out of range in a short month", false);
    }
	return true;
}

static bool validateInputLine(std::string& line, myDate& date, float& value)
{
    std::cout << std::setfill(' ') << std::left << std::setw(50) << line << " ";
    int size = line.size();
    int n = 0;
    int fields = std::sscanf(line.c_str(), "%d-%d-%d | %f%n", &date.year, &date.month, &date.day, &value, &n); // %u for unsigned?

	if (n < size && fields == 4)
        return err("❌ Error: bad input - extra character(s) found after value", false);
	if (n == 0 || fields < 4)
		return err("❌ Error: bad input - left unfilled fields in [Y/M/D | V] format", false);
	if (!performDateChecks(date))
		return false;
	if (value < 0 || value > 1000)
        return err("❌ Error: bad input - value out of range", false);
    return true;
}

static bool validateDataLine(std::string& line, myDate& date, float& value)
{
    int size = line.size();
    int n = 0;
    int fields = std::sscanf(line.c_str(), "%d-%d-%d,%f%n", &date.year, &date.month, &date.day, &value, &n); // %u for unsigned?

    if (n < size && fields == 4)
        return err("❌ Error: bad input - extra character(s) found after value", false);
    if (n == 0 || fields < 4)
        return err("❌ Error: bad input - left unfilled fields in [Y/M/D,V] format", false);
    if (!performDateChecks(date))
		return false;
    if (value < 0 || value > 1000000)
		return err("❌ Error: bad input - value out of range", false);
    return true;
}

static bool validateData(std::ifstream& db, std::map<myDate, float>& data)
{
	printHeader("Validating data...");
	std::string line;
    std::getline(db, line);
	if (line != "date,exchange_rate")
	{
		return err("❌ Error: wrong input header formatting", false);
	}
	myDate date;
	float value;
	while (std::getline(db, line))
	{
        if (!validateDataLine(line, date, value))
        {
            return err(" ❌ Error: [data.csv] cannot provide valid data", false);
        }
		for (std::map<myDate, float>::iterator it = data.begin(); it != data.end(); it++)
		{
			if (it->first == date)
			{
				return err(" ❌ Error: [data.csv] contains duplicate dates", false);
			}
		}
		data[date] = value;
	}
	printHeader("Data OK :)");
	return true;
}

static bool displayBTC(std::ifstream& input, std::map<myDate, float> &data)
{
	std::string line;
	std::getline(input, line);
	if (line != "date | value")
	{
		return err("❌ Error: wrong input header formatting", false);
	}
	myDate date;
    float value;
	while (std::getline(input, line))
	{
        if (!validateInputLine(line, date, value))
            continue;
        // display
        std::cout << std::endl;
	}
	(void)data;
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
	//  BitcoinExchange exchange;

    std::map<myDate, float> data;
    if (!validateData(db, data))
		return 1;
    
    // exchange.setData();
	if (!displayBTC(input, data))
		return 1;
	return 0;
}
