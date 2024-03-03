/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lsadiq <lsadiq@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/05 19:04:39 by kel-baam          #+#    #+#             */
/*   Updated: 2024/03/02 15:21:14 by lsadiq           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/Server.hpp"

Server::Server()
{
    error_pages[404] = "/nfs/homes/kel-baam/Desktop/kel/srcs/errorPages/404.html";
    // error_pages[405] = "../errorPages/405.html";
    // error_pages[301] = "../errorPages/301.html";
    // error_pages[500] = "../errorPages/500.html";
    // error_pages[501] = "../errorPages/501.html";
}

std::map<int, std::string> Server::getErrorPage()
{
    return this->error_pages;
}

void Server::setServerData(std::string key ,std::vector<std::string> vec)
{
    if(key != "error_page")
    {
        if(serverData.count(key) >= 1)
                throw duplicatedError();
        serverData[key].insert(serverData[key].end(),vec.begin(),vec.end()); 
    }
    else
        error_pages[atoi(vec[0].c_str())] = vec[1]; 
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
    if (index >= locations.size()) 
    {
        throw std::out_of_range("Index out of range");
    }
    return locations[index];
}

void Server::printLOcationINfo()
{
    // std::cout << "+++++ SERVER DATA +++++\n";
    // if(!serverData["port"].empty())
    //     std::cout << "  port"<<serverData["port"][0]<< "\n";
    // if(!serverData["host"].empty())
    //     std::cout << "  host"<<serverData["host"][0]<< "\n";
    // if(!serverData["server_name"].empty())
    //     std::cout << "  server_name"<<serverData["server_name"][0]<< "\n";
    // if(!serverData["client_max_body_size"].empty())
    //     std::cout << "  client_max_body_size"<<serverData["client_max_body_size"][0]<< "\n";
    // if(!serverData["root"].empty())
    //     std::cout << "  root"<<serverData["root"][0]<< "\n";
    for(size_t i = 0 ; i <locations.size();i++)
    {
        locations[i].printData();
    }
}

std::vector<Location>  Server::getLocations()
{            
    return locations;
}

//check overflow
void Server::checkPortError(std::vector<std::string> port)
{
    size_t len = port[0].length();
    size_t index = port[0].find_first_not_of("0123456789");
    
    if(port.size() != 1)
        throw portError();
    if(index!= std::string::npos ||  len > 5 || port[0][0] == '-' < 0 || atoi(port[0].c_str()) > 65535)
        throw portError();
}

void Server::checkHostError(std::vector<std::string> host)
{
    std::vector<std::string> nums;
    size_t index;

    if(host.size() != 1 || std::count(host[0].begin(),host[0].end(),'.') > 3)
        throw HostError();
    if(host[0] == "localhost" )
        return;
    nums = Utils::splitString(host[0],'.');
    for(size_t i = 0; i < nums.size();i++)
    {
        // chwck find not of 
        index = nums[i].find_first_not_of("0123456789");
        if(index!= std::string::npos || atoi(nums[i].c_str()) < 0 || atoi(nums[i].c_str()) > 255)
            throw HostError();
    }
}
// check what num in units and ovwrflow
void Server::checkClientMaxBody(std::vector<std::string> bodySize)
{
    if(bodySize.size() != 1 || bodySize[0][0] =='-')
        throw ClientMaxBodyError();
    // for(size_t i = 0; i < bodySize.size();i++)
    // {
    //     index = bodySize[i].find_first_not_of("0123456789");
    //     if(index!= std::string::npos)
    //         throw HostError();
    // }
}

//is i should check forn another valid caracter
void Server::checkServernameError(std::vector<std::string> serverName)
{
    if(serverName.size() != 1)
        throw serverNameError();
}
// is should i check this

void Server::checkErrorPages(std::vector<std::string> vec)
{
    if(vec.size() != 2 ) // negative
        throw errorPages();
    
}

void Server::checkServersError(std::string directive, std::vector<std::string> vec,int countTabulation)
{ 
    (void)countTabulation;
    if(countTabulation != 1)
        throw  ConfigueFileError();
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
        throw  ConfigueFileError();

}