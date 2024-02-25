/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kel-baam <kel-baam@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/05 19:04:39 by kel-baam          #+#    #+#             */
/*   Updated: 2024/02/22 15:10:07 by kel-baam         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/server.hpp"



Server::Server()
{
        serverData["port"].push_back("80");
        serverData["root"].push_back("/var/www/html");
        serverData["index"].push_back("index.html");
        serverData["server_name"].push_back("localhost");
        //maybe i sould set a default client body max 
}

void Server::setServerData(std::string key ,std::vector<std::string> vec)
{
    serverData[key].insert(serverData[key].begin(),vec.begin(),vec.end()); 
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

bool  Server::isValidBodySize(std::string size)
{
    return true;
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
    if(!serverData["port"].empty())
        std::cout << "  port"<<serverData["port"][0]<< "\n";
    if(!serverData["host"].empty())
        std::cout << "  host"<<serverData["host"][0]<< "\n";
    if(!serverData["server_name"].empty())
        std::cout << "  server_name"<<serverData["server_name"][0]<< "\n";
    if(!serverData["client_max_body_size"].empty())
        std::cout << "  client_max_body_size"<<serverData["client_max_body_size"][0]<< "\n";
    if(!serverData["root"].empty())
        std::cout << "  root"<<serverData["root"][0]<< "\n";
    for(size_t i = 0 ; i <locations.size();i++)
    {
        locations[i].printData();
    }
}

void checkMembershipInServer(std::string directive)
{
    
    // if(dircetive != "root" && directive != "port" && directive != "host" && dircetive!="server_name" && 
    // dircetive!= "client_max_body_size" && directive != "error_page")
    //     throw directiveError();
}
std::vector<Location>  Server::getLocations()
{            
    return locations;
}

void Server::checkPortError(std::vector<std::string> port)
{
    char *store;
    size_t len = port[0].length();
    size_t index = port[0].find_first_not_of("0123456789");
    
    if(port.size() > 1)
        throw portError();
    if(index!= std::string::npos ||  len > 5 || atoi(port[0].c_str()) < 0 || atoi(port[0].c_str()) > 65535)
        throw portError();
        
}

void Server::checkHostError(std::vector<std::string> host)
{
    std::vector<std::string> nums;
    size_t index;
    if(host.size() > 1 || std::count(host[0].begin(),host[0].end(),'.') > 3)
        throw HostError();
    if(host[0] == "localhost" )
        return;
    nums = Utils::splitString(host[0],'.');
    for(size_t i = 0; i < nums.size();i++)
    {
        // std::cout << nums[i] << std::endl;
        index = nums[i].find_first_not_of("0123456789");
        if(index!= std::string::npos || atoi(nums[i].c_str()) < 0 || atoi(nums[i].c_str()) > 255)
            throw HostError();
    }
}

void Server::checkClientMaxBody(std::vector<std::string> bodySize)
{
    if(bodySize.size()> 1)
        throw ClientMaxBodyError();
    // if()
}

void Server::checkServernameError(std::vector<std::string> serverName)
{
    // if(serverName.size() > 1)
    //     throw serverNameError();
}
void Server::checkRoot(std::vector<std::string> root)
{
     DIR* directory;
    // std::cout << root[0] << "|\n";
    if(root.size() > 1 ||root[0][0] != '/')
        throw rootError();
    directory = opendir(root[0].c_str());
    if(!directory)
        throw rootError();
    closedir(directory);  
}
void Server::checkServersError(std::string directive, std::vector<std::string> vec,int countSpaces)
{
    if(countSpaces != 1)
        throw  ConfigueFileError();
    if(directive == "root")
        checkRoot(vec);
    if(directive == "client_max_body_size")
        checkClientMaxBody(vec);
    else if(directive == "port")
        checkPortError(vec);
    else if(directive == "host")
        checkHostError(vec);
    else if(directive == "server_name")
        checkServernameError(vec);
}