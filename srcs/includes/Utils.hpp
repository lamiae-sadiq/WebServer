/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kel-baam <kel-baam@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/14 13:52:12 by kel-baam          #+#    #+#             */
/*   Updated: 2024/02/22 17:04:35 by kel-baam         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#pragma once

#include <iostream>
#include <vector>
#include <map>
#include <string>
#include <fstream>
#include <exception>



class Utils
{
	public:
   		static std::vector<std::string> splitString(std::string string,int del);
		static int  countTabulation(std::string &line);
		static void skipSpaces(std::string &line);
		static int isWhitSpaces(int a);
	
};