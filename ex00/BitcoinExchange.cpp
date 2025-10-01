/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   BitcoinExchange.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vsanin <vsanin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/16 14:50:20 by vsanin            #+#    #+#             */
/*   Updated: 2025/10/01 20:55:03 by vsanin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "BitcoinExchange.hpp"
#include <cstdlib>
#include <iostream>
#include <iomanip>

/*================================[ OCF ]=====================================*/

BitcoinExchange::BitcoinExchange() : data() {}

BitcoinExchange::BitcoinExchange(std::map<myDate, float>& db) : data(db) {}

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

std::map<myDate, float>& BitcoinExchange::getData(void)
{
	return data;
}

void BitcoinExchange::setData(const std::map<myDate, float>& dt)
{
    data = dt;
}

/*===============================[ UTILS ]====================================*/

static bool err(std::string msg, bool ret)
{
    std::cerr << msg << "\n";
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

/*==========================[ DATA VALIDATION ]===============================*/

bool BitcoinExchange::validateData(std::ifstream& db, std::map<myDate, float>& data)
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
        if (!BitcoinExchange::validateDataLine(line, date, value))
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

bool BitcoinExchange::validateDataLine(std::string& line, myDate& date, float& value)
{
    int size = line.size();
    int n = 0;
    int fields = std::sscanf(line.c_str(), "%d-%d-%d,%f%n", &date.year, &date.month, &date.day, &value, &n);

    if (n < size && fields == 4)
        return err("❌ Error: bad input - extra character(s) found after value", false);
    if (n == 0 || fields < 4)
        return err("❌ Error: bad input - left unfilled fields in [Y/M/D,V] format", false);
    if (!BitcoinExchange::performDateChecks(date))
		return false;
    if (value < 0 || value > 1000000)
		return err("❌ Error: bad input - value out of range", false);
    return true;
}

bool BitcoinExchange::validateInputLine(std::string& line, myDate& date, float& value)
{
    std::cout << std::setfill(' ') << std::left << std::setw(38) << line << " ";
    int size = line.size();
    int n = 0;
    int fields = std::sscanf(line.c_str(), "%d-%d-%d | %f%n", &date.year, &date.month, &date.day, &value, &n);

	if (n < size && fields == 4)
        return err("❌ Error: bad input - extra character(s) found after value", false);
	if (n == 0 || fields < 4)
		return err("❌ Error: bad input - left unfilled fields in [Y/M/D | V] format", false);
	if (!BitcoinExchange::performDateChecks(date))
		return false;
	if (value < 0 || value > 1000)
        return err("❌ Error: bad input - value out of range", false);
    return true;
}

bool BitcoinExchange::performDateChecks(myDate& date)
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

/*==============================[ DISPLAY ]===================================*/

bool BitcoinExchange::displayBTC(std::ifstream& input, std::map<myDate, float> &data)
{
	std::string line;
	std::getline(input, line);
	if (line != "date | value")
	{
		return err("❌ Error: wrong input header formatting", false);
	}
	printHeader("Bitcoin Exchange");
	myDate date;
    float value;
	while (std::getline(input, line))
	{
        if (!BitcoinExchange::validateInputLine(line, date, value))
            continue;
		std::map<myDate, float>::iterator it = data.find(date);
		if (it == data.end())
		{
			BitcoinExchange::findNearestDate(date, data, value, it);
		}
		else
		{
			std::cout << "✅ " << it->second << " x " << value << " = " << value * it->second << std::endl;
		}
	}
	return true;
}

void BitcoinExchange::findNearestDate(myDate& date, std::map<myDate, float> &data, float& value, std::map<myDate, float>::iterator& it)
{
	int foundFlag = 0;
	for (int y = date.year; y > 2008; y--)
	{
		date.year = y;
		for (int m = date.month; m > 0; m--)
		{
			date.month = m;
			for (int d = date.day; d > 0; d--)
			{
				date.day = d;
				it = data.find(date);
				if (it != data.end())
				{
					std::cout << "✅ " << it->second << " x " << value << " = " << value * it->second
					<< " (based on exchange rate on " << date.year << "-" << date.month << "-" << date.day << ")\n";
					foundFlag = 1;
					break;
				}
			}
			if (foundFlag)
				break;
			date.day = 31;
		}
		if (foundFlag)
			break;
		date.month = 12;
	}
	if (!foundFlag)
		err("❌ Error: no previous data", false);
}
