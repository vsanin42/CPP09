/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   BitcoinExchange.hpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vsanin <vsanin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/11 18:56:42 by vsanin            #+#    #+#             */
/*   Updated: 2025/10/01 20:49:40 by vsanin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BITCOINEXCHANGE_HPP
#define BITCOINEXCHANGE_HPP
#include <map>
#include <string>
#include <fstream>

struct myDate{
	int year, month, day;
	bool operator<(const myDate& rhs) const
	{
		if (year != rhs.year) return year < rhs.year;
		if (month != rhs.month) return month < rhs.month;
		return day < rhs.day;
	};
	bool operator==(const myDate& rhs) const
	{
		if (year == rhs.year && month == rhs.month && day == rhs.day) return true;
		return false;
	};
};

class BitcoinExchange
{
	private:
		std::map<myDate, float> data;
		static bool validateDataLine(std::string& line, myDate& date, float& value);
		static bool performDateChecks(myDate& date);
		static bool validateInputLine(std::string& line, myDate& date, float& value);
		static void findNearestDate(myDate& date, std::map<myDate, float> &data, float& value, std::map<myDate, float>::iterator& it);
	public:
		BitcoinExchange();
		BitcoinExchange(std::map<myDate, float>& db);
		BitcoinExchange(const BitcoinExchange& ref);
		BitcoinExchange& operator=(const BitcoinExchange& ref);
		~BitcoinExchange();

		std::map<myDate, float>& getData(void);
        void setData(const std::map<myDate, float>& dt);

		static bool validateData(std::ifstream& db, std::map<myDate, float>& data);
		bool displayBTC(std::ifstream& input, std::map<myDate, float> &data);
};

#endif
