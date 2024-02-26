/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kel-baam <kel-baam@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/03 15:14:16 by kel-baam          #+#    #+#             */
/*   Updated: 2024/02/24 14:15:13 by kel-baam         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/Parse.hpp"


Parser::Parser()
{
  locationLen = -1;
  serverLen = -1;
  
}

void Parser::serversAddBack()
{
  Server server;
  servers.push_back(server);
}

void  Parser::printServersInfo()
{
  for(size_t i = 0; i < servers.size();i++)
  {
    std::cout << "siiize" << servers[i].getSizeServerData() << std::endl;
    servers[i].printLOcationINfo();    
  }
}

void   Parser::storeServerData(std::string &directive, std::string &line, int countTab,Server &servers)
{
  Server::checkServersError(directive,Utils::splitString(line,' '),countTab);
  servers.setServerData (directive,Utils::splitString(line,' '));
 }


void Parser::analyseLocationData(std::string &line,Server &servers,std::ifstream &readFile)
{
  std::string directive;
  size_t index;
  int countTab;
  
  locationLen++;
  servers.locationAddBack();
  while(std::getline(readFile,line))
  {
    if(!line.empty())
    {
      countTab  = Utils::countTabulation(line);
      directive = line;
      index = line.find(' ');
      if(index!=std::string::npos)
      {
        directive = line.substr(0,index);
        line = line.substr(index + 1);
      }
      if(directive == "location" || directive == "server")
        break;
      Location::checkLocationError(directive,Utils::splitString(line,' '),countTab);
      servers[locationLen].setLocationData(directive,Utils::splitString(line,' '));
    }
  }
}

void Parser::analyseServerData(std::string &line,std::ifstream& readFile,Server &server)
{
  size_t countTab;
  size_t index;
  std::string directive;
  
  while(line != "location" && std::getline(readFile,line))// he read from the file && stop when he fin location
  {
    if(!line.empty())
    {
      countTab  = Utils::countTabulation(line);
      index = line.find(' ');
      directive = line;
      if(index != std::string::npos)
      {
        directive = line.substr(0,index);//leaks
        line = line.substr(index + 1);
      }
      if(directive != "location")
        storeServerData(directive, line,countTab,server);
    }
  }
}

void Parser::readFile(std::string line,std::ifstream& readFile)
{
  while(!readFile.eof())
  {
    if(line == "server")
    {
      serversAddBack();
      serverLen++;
      locationLen = -1;
    }
    if(!line.empty() || line !="\n")
    {
      if (line != "location")
        analyseServerData(line,readFile,servers[serverLen]);
      else
        analyseLocationData(line,servers[serverLen],readFile);
    }
  }
  //chack the necassry directive;
  // if(readFile.eof())
  // {
  //     printServersInfo();
  // }
}

std::vector<Server> Parser::paseConfigueFile(std::string &configueFile)
{
  std::string line;
  std::ifstream readFile;
  Parser parser;
  
  readFile.open(configueFile.c_str());
  std::getline(readFile,line);
  if(line == "server")
  {
    parser.readFile(line,readFile);
    
  }
  else
    throw ConfigueFileError();
  return parser.servers;
}

	