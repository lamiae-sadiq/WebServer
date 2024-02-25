/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Location.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kel-baam <kel-baam@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/07 09:07:35 by kel-baam          #+#    #+#             */
/*   Updated: 2024/02/24 16:27:32 by kel-baam         ###   ########.fr       */
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
#define GET_METHODE "GET"
#define POST_METHODE "POST"
#define DELETE_METHODE "DELETE"

class Location
{
        private:
                std::map<std::string,std::vector<std::string> > locationData;
                // bool allowedUpload;
                // struct Loca loc;
        public:
                Location();
                //static void checkUpload(std::vector<std::string> allowedApload);
                void setLocationData(std::string key, std::vector<std::string> vec);
                std::vector<std::string> getLocationData(std::string key);
                void printData();
                static void   checkMembershipInLocation(std::string directive);
                static void checkLocationError(std::string directive,std::vector<std::string> vec,int countSpaces);
                static bool isValidPath(std::string location_name);
                static void checkAutoindexError(std::vector<std::string> autoindex);
                static void checkMethodsError(std::vector<std::string> methods); 
                static void  checkPathError(std::vector<std::string> location_name);              
                ~Location();
                class autoindexError:public std::exception 
                {
                        const char* what() const throw()
                        {
                                return "you have an error in autoindex\n";
                        }
                };
                class httpMethodsError:public std::exception 
                {
                        const char* what() const throw()
                        {
                                return "you have an error in http methods\n";
                        }
                };
                class ConfigueFileError:public std::exception 
                {
                        const char* what() const throw()
                        {
                                return "you have an error in your configue file\n";
                        }
                };
                class directiveError:public std::exception 
                {
                        const char* what() const throw()
                        {
                                return "you have an error in directives name \n";
                        }
                };
                class location_nameError:public std::exception 
                {
                        const char* what() const throw()
                        {
                                return "you have an error in location location_name \n";
                        }
                };
};


