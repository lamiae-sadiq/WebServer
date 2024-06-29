/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Parse.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kel-baam <kel-baam@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/03 15:14:18 by kel-baam          #+#    #+#             */
/*   Updated: 2024/03/23 23:47:51 by kel-baam         ###   ########.fr       */
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
        void  storeServerData(std::string &directive, std::string &line, int countTab,Server &servers);
        void analyseLocationData(std::string &line,Server &servers,std::ifstream &raedFile);
        void analyseServerData(std::string& line,std::ifstream& readFile,Server& server);
        void checkFinalData(std::vector<Server> servers);
    public:
        static std::vector<Server> paseConfigueFile(std::string &configueFile);
};

