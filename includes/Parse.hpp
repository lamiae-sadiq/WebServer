/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Parse.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kel-baam <kel-baam@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/03 15:14:18 by kel-baam          #+#    #+#             */
/*   Updated: 2024/03/19 12:37:01 by kel-baam         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once


#include <iostream>
#include <vector>
#include <map>
#include <string>
#include <fstream>
#include <exception>
#include "./Server.hpp"
#include "./Location.hpp"
#include "./Utils.hpp"
// ishould check if the server data is four or not 

class Parser
{
    private:
        std::vector<Server> servers;
        int locationLen;
        int serverLen;
        
        Parser();
        ~Parser();
        void readFile(std::string line, std::ifstream &readFile);
        void serversAddBack();
        void printServersInfo();
        void  storeServerData(std::string &directive, std::string &line, int countTab,Server &servers);
        void analyseLocationData(std::string &line,Server &servers,std::ifstream &raedFile);
        std::vector<std::string> splitString(std::string &string,int del);
        void analyseServerData(std::string& line,std::ifstream& readFile,Server& server);
        void checkFinalData(std::vector<Server> servers);
    public:
        static std::vector<Server> paseConfigueFile(std::string &configueFile);
};

