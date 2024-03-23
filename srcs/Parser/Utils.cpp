/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Utils.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kel-baam <kel-baam@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/14 13:55:29 by kel-baam          #+#    #+#             */
/*   Updated: 2024/03/23 22:44:48 by kel-baam         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/Utils.hpp"

std::vector<std::string> Utils::splitString(std::string string,int del)
{
	std::vector<std::string> vec;
	std::string tmpString = string;
	size_t index = 1;
  
	while(index !=std::string::npos)
	{	
    
		index = string.find(del);
    
		if(index != std::string::npos)
		{
			vec.push_back(string.substr(0,index));
      string = string.substr(index +1);
		}
	}
	vec.push_back(string);
    return vec;
}

int  Utils::isWhitSpaces(int a)
{
  return (a == ' ' || a =='\t');
}

void  Utils::skipSpaces(std::string &line)
{
  size_t start;
  size_t end;
  size_t len = line.length() - 1;
  int count = 0;
  for(start = 0;start < line.length();start++)
  {
    if(!isWhitSpaces(line[start]))
      break;
  }
  for(end = len ; end >=0; end--)
  {
    if(!isWhitSpaces(line[end]))
      break;
    count++;
  }
  line = line.substr(start,len - start - count + 1); 
}

//also in the end
int  Utils::countTabulation(std::string &line)
{
  int count = 0;
  int end = 0;

  int last = 0;

  size_t len = line.length() - 1;

  for(size_t i = 0 ; i < len ;i++)
  {
    if(line[i] !='\t')
      break;
    count +=1;
  }
  for(end = len; end >= 0; end--)
  {
    if(line[end] != ' ' && line[end] != '\t')
      break;
    last++;
  }
  line = line.substr(count,len - count - last + 1);
  return count;
}

bool Utils::isHex(char ch)
{
  std::string characters = "abcdefABCDEF";
  if(isdigit(ch))

    return true;
  for(size_t i = 0; i <characters.length();i++)
  {
    if(characters[i] == ch)
      return true;
  }
  return false;
}

bool Utils::isInteger(std::string num)
{
  for(size_t i = 0; i< num.length();i++)
  {
    if(!isdigit(num[i]) && num[i] != '.')
      return false;
  }
  return true;
}
void Utils::strToLower(std::string &str)
{
  for(size_t i = 0; i< str.length(); i++)
  {
    str[i] = tolower(str[i]);
  }
}

long long   Utils::stringToLongLong(std::string str)
{
    long long  num;
    std::stringstream ToLong;
    ToLong << str ;
    ToLong >> num;
    return num;
}

bool  Utils::checkOverflowError(std::string originNum, long long  currentNum)
{
    std::stringstream ToString;
    std::string currentString;

    ToString << currentNum;
    ToString >> currentString;
    if(currentString != originNum)
        return false;
    return true;
}