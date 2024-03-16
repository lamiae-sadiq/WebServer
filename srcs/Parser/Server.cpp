/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kel-baam <kel-baam@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/05 19:04:39 by kel-baam          #+#    #+#             */
/*   Updated: 2024/03/16 13:56:58 by kel-baam         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/Server.hpp"

Server::Server()
{
    error_pages[404] = "/nfs/homes/kel-baam/del/srcs/errorPages/404.html";
    error_pages[405] = "/nfs/homes/kel-baam/del/srcs/errorPages/405.html";
    error_pages[301] = "/nfs/homes/kel-baam/del/srcs/errorPages/301.html";
    error_pages[500] = "/nfs/homes/kel-baam/del/srcs/errorPages/500.html";
    error_pages[501] = "/nfs/homes/kel-baam/del/srcs/errorPages/501.html";
}
Server::~Server(){};

std::map<int, std::string> Server::getErrorPage()
{
    return this->error_pages;
}

void Server::setServerData(std::string key ,std::vector<std::string> vec)
{
    if(key != "error_page")
    {
        if(serverData.count(key) >= 1)
             throw Exception("Error: one of the dirictives is duplicated\n");
        serverData[key].insert(serverData[key].end(),vec.begin(),vec.end()); 
    }
    else
        error_pages[Utils::stringToLongLong(vec[0].c_str())] = vec[1]; 
}

void  Server::isValidLocations()
{
    bool defaultLocation = false;
    for(size_t i = 0; i < this->getLocations().size();i++)
    {
        if(!this->getLocations()[i].isValidLocation())
            throw Exception("Error: the necessary dircetives in the location is missing\n");
        if(this->getLocations()[i].getLocationData("location_name")[0] == "/")
            defaultLocation = true; 
    }
    if(!defaultLocation)
        throw Exception("Error: default location is missing");
}

void Server::locationAddBack()
{
    Location location;
    locations.push_back(location);
}

std::vector<std::string>  Server::getServerData(std::string key)
{
    std::vector<std::string> tmp;
    std::map<std::string,std::vector<std::string> >::iterator it;
    
    it = serverData.find(key);
    if(it!= serverData.end())
        tmp = serverData[key];
    return tmp;
}

size_t  Server::directiveSize(std::string directive)
{
    return serverData[directive].size();
}

Location& Server::operator[](size_t index)
{
    if (index >= locations.size() || index < 0) 
        throw Exception("index is out of range\n");
    return locations[index];
}

std::vector<Location>  Server::getLocations()
{            
    return locations;
}

bool  Server::isValidServer()
{
    if(serverData.count("port") == 1 && serverData.count("host") == 1 && serverData.count("client_max_body_size") == 1)
        return true;
    return false;
}

void Server::checkPortError(std::vector<std::string> port)
{
    size_t len = port[0].length();
    size_t index = port[0].find_first_not_of("0123456789");
    long long int portNum = Utils::stringToLongLong(port[0]);
    if(port.size() != 1)
        throw Exception("Error: port is incorrect");
    if(!Utils::checkOverflowError(port[0],portNum))
      	throw Exception("Error: port is incorrect");
    if(index!= std::string::npos ||  len > 5 || port[0][0] == '-' < 0 || portNum > 65535)
        throw  Exception("Error: port is incorrect");
}

void Server::checkHostError(std::vector<std::string> host)
{
    std::vector<std::string> nums;
    size_t index;
    long long int hostNum;
    
    if(host.size() != 1 || std::count(host[0].begin(),host[0].end(),'.') > 3)
        throw Exception("Error: host is incorrect\n");
    if(host[0] == "localhost" )
        return;
    nums = Utils::splitString(host[0],'.');
    for(size_t i = 0; i < nums.size();i++)
    {
        index = nums[i].find_first_not_of("0123456789");
        hostNum = Utils::stringToLongLong(nums[i]);
        if(index!= std::string::npos || hostNum < 0 || hostNum > 255)
            throw Exception("Error: host is incorrect\n");
        if(!Utils::checkOverflowError(nums[i],hostNum))
            throw Exception("Error: host is incorrect\n");
    }
}

void Server::checkClientMaxBody(std::vector<std::string> bodySize)
{
    long long int maxSize = Utils::stringToLongLong(bodySize[0].c_str());
    if(!Utils::checkOverflowError(bodySize[0],maxSize))
        throw Exception("Error: max_body_size is incorrect\n");
    if(maxSize < 0)
        throw Exception("Error: max_body_size is incorrect");
    if(bodySize.size() != 1 || bodySize[0].find_first_not_of("0123456789")!= std::string::npos)
        throw Exception("Error: max_body_size is incorrect\n");
}

void Server::checkServernameError(std::vector<std::string> serverName)
{
    if(serverName.size() != 1)
        throw Exception("Error: server name is incorrect\n");
}

void Server::checkErrorPages(std::vector<std::string> vec)
{
    long long errorNum;
    
    if(vec.size() != 2 || vec[0].find_first_not_of("0123456789")!= std::string::npos || Utils::isWhitSpaces(vec[1][0]))
        throw Exception("Error: error pages is incorrect\n");
    errorNum = Utils::stringToLongLong(vec[0].c_str());
    if(!Utils::checkOverflowError(vec[0], errorNum))
        throw Exception("Error: error pages\n");
    if(errorNum < 400 || errorNum > 599)
        throw Exception("Error: error pages\n");
}

void Server::checkServersError(std::string directive, std::vector<std::string> vec,int countTabulation)
{ 
    if(countTabulation != 1)
        throw Exception("Error: configuefile format is incorrect\n");
    if(directive == "error_page")
        checkErrorPages(vec);
    else if(directive == "client_max_body_size")
        checkClientMaxBody(vec);
    else if(directive == "port")
        checkPortError(vec);
    else if(directive == "host")
        checkHostError(vec);
    else if(directive == "server_name")
        checkServernameError(vec);
    else
        throw  Exception("Error: directive name is incorrect\n");
}
