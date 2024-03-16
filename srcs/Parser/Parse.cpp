/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Parse.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kel-baam <kel-baam@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/03 15:14:16 by kel-baam          #+#    #+#             */
/*   Updated: 2024/03/15 14:21:52 by kel-baam         ###   ########.fr       */
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

void   Parser::storeServerData(std::string &directive, std::string &line, int countTab,Server &servers)
{
  Utils::splitString(line,' ');
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
      if((directive == "location" && countTab != 1) || (directive == "server" && countTab != 0))
        throw ConfigueFileError();
      if(directive == "location" || directive == "server")
        break;
      Location::checkLocationError(directive,Utils::splitString(line,' '),countTab);
      if(directive == "location_name")
      {
        for(size_t i = 0;i < servers.getLocations().size() - 1;i++)
        {
          if(servers.getLocations()[i].getLocationData("location_name")[0] == line)
            throw ConfigueFileError();
        }
      }
      servers[locationLen].setLocationData(directive,Utils::splitString(line,' '));
    }
  }
}

void Parser::analyseServerData(std::string &line,std::ifstream& readFile,Server &server)
{
  size_t countTab;
  size_t index;
  std::string directive;
  
  while(line != "location" && std::getline(readFile,line))
  {
    if(!line.empty())
    {
      countTab  = Utils::countTabulation(line);
      index = line.find(' ');
      directive = line;
      if(index != std::string::npos)
      {
        directive = line.substr(0,index);
        line = line.substr(index + 1);
      }
      if(directive != "location")
        storeServerData(directive, line,countTab,server);
      if(directive == "location" && countTab != 1)
        throw ConfigueFileError();
    }
  }
}

void Parser::checkFinalData(std::vector<Server> servers)
{
  for(size_t i = 0; i < servers.size(); i++)
  {
    if(!servers[i].isValidServer())
      throw ConfigueFileError();
    servers[i].isValidLocations();
  }
}

void Parser::readFile(std::string line,std::ifstream& readFile)
{
  std::getline(readFile,line);
  if(line != "server")
    throw ConfigueFileError();
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
  checkFinalData(servers);
}

std::vector<Server> Parser::paseConfigueFile(std::string &configueFile)
{
  std::string line;
  std::ifstream readFile;
  Parser parser;
  
  readFile.open(configueFile.c_str());
  if(readFile.is_open())
    parser.readFile(line,readFile);
  else
    throw ConfigueFileError();
  return parser.servers;
}

	