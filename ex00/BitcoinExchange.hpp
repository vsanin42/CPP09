/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   BitcoinExchange.hpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vsanin <vsanin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/11 18:56:42 by vsanin            #+#    #+#             */
/*   Updated: 2025/09/29 13:47:18 by vsanin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BITCOINEXCHANGE_HPP
#define BITCOINEXCHANGE_HPP
#include <map>
#include <string>
#include <fstream>

class BitcoinExchange
{
	private:
		// std::map<std::string, float> input;
		std::map<std::string, float> data;
	public:
		BitcoinExchange();
		BitcoinExchange(std::ifstream& db);
		BitcoinExchange(const BitcoinExchange& ref);
		BitcoinExchange& operator=(const BitcoinExchange& ref);
		~BitcoinExchange();
		// std::map<std::string, float>& getInput(void);
		std::map<std::string, float>& getData(void);
};

#endif
