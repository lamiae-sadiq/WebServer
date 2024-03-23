/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Location.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lsadiq <lsadiq@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/07 09:07:35 by kel-baam          #+#    #+#             */
/*   Updated: 2024/03/23 21:41:55 by lsadiq           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include <iostream>
#include <vector>
#include <map>
#include <string>
#include <fstream>
#include <exception>
#include <algorithm> 
#include "Utils.hpp"
#include "Exception.hpp"
#define GET_METHODE "GET"
#define POST_METHODE "POST"
#define DELETE_METHODE "DELETE"
#define ON "on"
#define OFF "off"

class Location
{
        private:
                std::map<std::string,std::vector<std::string> > locationData;
        public:
                Location();
                void setLocationData(std::string key, std::vector<std::string> vec);
                std::vector<std::string> getLocationData(std::string key);
                static void checkLocationError(std::string directive,std::vector<std::string> vec,int countSpaces);
                static void checkAutoindexError(std::vector<std::string> autoindex);
                static void checkMethodsError(std::vector<std::string> methods); 
                static void  checkLocationName(std::vector<std::string> location_name);   
                static void checkAllowedUpload(std::vector<std::string> allowedApload);
                static void checkIndexError(std::vector<std::string> index);
                static void checkReturnError(std::vector<std::string> returnDirective);
                static void checkRootError(std::vector<std::string> root);
                static void checkUpload(std::vector<std::string> upload);
                static void checkCgiError(std::vector<std::string> vec);
                bool isValidLocation();
                ~Location();
};


