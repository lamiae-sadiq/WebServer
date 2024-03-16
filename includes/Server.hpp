/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kel-baam <kel-baam@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/05 19:46:06 by kel-baam          #+#    #+#             */
/*   Updated: 2024/03/16 13:54:11 by kel-baam         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#pragma once

#include <iostream>
#include <vector>
#include <map>
#include <string>
#include <fstream>
#include <exception>
#include "./Location.hpp"
#include <cstdlib>
#include "./Utils.hpp"
#include <algorithm> 
#include <sys/types.h>
#include <dirent.h>
#include "Exception.hpp"
typedef std::map<std::string,std::vector<std::string> > mapOfVectors;
class Server
{
        mapOfVectors serverData;
        std::map<int, std::string> error_pages;
        int countServerData;
        std::vector<Location> locations;
    public:
        Server();
        ~Server();
        std::map<int, std::string> getErrorPage();
        void setServerData(std::string key ,std::vector<std::string> vec);
        std::vector<std::string> getServerData(std::string key);
        void        printLOcationINfo();
        size_t      directiveSize(std::string directive);
        void        locationAddBack();
        static void checkErrorPages(std::vector<std::string> vec);
        static void checkServernameError(std::vector<std::string> serverName);
        static void checkPortError(std::vector<std::string> port);
        static void checkHostError(std::vector<std::string> host);
        static void checkClientMaxBody(std::vector<std::string> bodySize);
        static void checkServersError(std::string directive, std::vector<std::string> vec,int countSpaces);
        std::vector<Location> getLocations();
        bool isValidServer();
        void isValidLocations();
        Location& operator[](size_t index);
};
