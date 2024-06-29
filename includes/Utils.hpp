/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Utils.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kel-baam <kel-baam@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/14 13:52:12 by kel-baam          #+#    #+#             */
/*   Updated: 2024/03/23 22:44:00 by kel-baam         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#pragma once

#include <iostream>
#include <vector>
#include <map>
#include <string>
#include <fstream>
#include <exception>
#include <string>
#include <iostream> 
#include <sstream> 

class Utils
{
	public:
   		static std::vector<std::string> splitString(std::string string,int del);
		static int  countTabulation(std::string &line);
		static void skipSpaces(std::string &line);
		static int isWhitSpaces(int a);
		static bool isInteger(std::string num);
		static void strToLower(std::string &str);
		static bool isHex(char ch);	
		static long long   stringToLongLong(std::string str);
		static bool  checkOverflowError(std::string originNum, long long  currentNum);
};