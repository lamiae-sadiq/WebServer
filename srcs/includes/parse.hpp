/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kel-baam <kel-baam@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/03 15:14:18 by kel-baam          #+#    #+#             */
/*   Updated: 2024/02/22 11:45:59 by kel-baam         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once


#include <iostream>
#include <vector>
#include <map>
#include <string>
#include <fstream>
#include <exception>
#include "./server.hpp"
#include "./Location.hpp"
#include "./utils.hpp"
// ishould check if the server data is four or not 

class Parser
{
    private:
        std::vector<Server> servers;
        int locationLen;
        int serverLen;
        Parser();
        void readFile(std::string line, std::ifstream &readFile);
        void serversAddBack();
        void printServersInfo();
        void  storeServerData(std::string &directive, std::string &line, int countTab,Server &servers);
        void analyseLocationData(std::string &line,Server &servers,std::ifstream &raedFile);
        void checkLocationError(std::string directive, std::vector<std::string> vec,Server &server);
        std::vector<std::string> splitString(std::string &string,int del);
        void analyseServerData(std::string& line,std::ifstream& readFile,Server& server);
    public:
        static std::vector<Server> paseConfigueFile(std::string &configueFile);
        class ConfigueFileError:public std::exception 
        {
            const char* what() const throw()
            {
                return "you have an error in your configue file\n";
            }
        };
};

