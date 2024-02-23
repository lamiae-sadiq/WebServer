/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kel-baam <kel-baam@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/14 13:55:29 by kel-baam          #+#    #+#             */
/*   Updated: 2024/02/22 17:16:10 by kel-baam         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/utils.hpp"
std::vector<std::string> Utils::splitString(std::string &string,int del)
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
  return (a == ' ' || a == '\n'  || a =='\t');
}

void  Utils::skipSpaces(std::string &line)
{
  size_t i;
  for( i = 0;i < line.length();i++)
  {
    std::cout<< i << "ddd\n";
    if(!isWhitSpaces(line[i]))
      break;
  }
  line = line.substr(i); 
}

//also in the end
int  Utils::countTabulation(std::string &line)
{
  int count = 0;
  int end = 0;

  int last = 0;
  size_t len = line.length() - 1;
  // for(size_t i = 0 ; i <len ;i++)
  // {
  //   if(line[i] != ' ')
  //     break;
  //   count++;
  // }
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