/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kel-baam <kel-baam@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/05 19:46:06 by kel-baam          #+#    #+#             */
/*   Updated: 2024/02/23 14:12:39 by kel-baam         ###   ########.fr       */
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
#include "./utils.hpp"
#include <algorithm> 
#include <sys/types.h>
#include <dirent.h>

typedef std::map<std::string,std::vector<std::string> > mapOfVectors;
class Server
{
        //SERVER DATA LIKE PORT HOST....
        mapOfVectors serverData; 
        // LOCATION DATA
        std::vector<Location> locations;
    public:
        Server();
        ~Server(){};
        void setServerData(std::string key ,std::vector<std::string> vec);
        std::vector<std::string> getServerData(std::string key);
        void checkServersError();
        void printLOcationINfo();
        size_t directiveSize(std::string directive);
        bool isValidBodySize(std::string size);
        bool isValidPort(std::string &port);
        bool isValidHost(std::string host);
        void locationAddBack();
        static void checkServernameError(std::vector<std::string> serverName);
        static void checkPortError(std::vector<std::string> port);
        static void checkHostError(std::vector<std::string> host);
        static void checkClientMaxBody(std::vector<std::string> bodySize);
        static void checkServersError(std::string directive, std::vector<std::string> vec,int countSpaces);
        std::vector<Location> getLocations();
        Location& operator[](size_t index);
        static void checkRoot(std::vector<std::string> root);
        class ConfigueFileError:public std::exception 
        {
            const char* what() const throw()
            {
                return "you have an error in your configue file\n";
            }
        };
        class portError:public std::exception 
        {
            const char* what() const throw()
            {
                return "you have an error in port\n";
            }
        };
        class HostError:public std::exception 
        {
            const char* what() const throw()
            {
                return "you have an error in you host\n";
            }
        };
        class ClientMaxBodyError:public std::exception 
        {
            const char* what() const throw()
            {
                return "you have an error in client body max size\n";
            }
        };
        class serverNameError:public std::exception 
        {
            const char* what() const throw()
            {
                return "you have an error in your server name\n";
            }
        };
        class rootError:public std::exception 
        {
            const char* what() const throw()
            {
                return "you have an error in your root location_name\n";
            }
        };
        class directiveError:public std::exception 
        {
            const char* what() const throw()
            {
                return "you have an error in directives name \n";
            }
        };
};
