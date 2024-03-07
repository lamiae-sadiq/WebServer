/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Location.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kel-baam <kel-baam@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/07 09:07:35 by kel-baam          #+#    #+#             */
/*   Updated: 2024/03/06 16:13:02 by kel-baam         ###   ########.fr       */
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
                void printData();
                static void   checkMembershipInLocation(std::string directive);
                static void checkLocationError(std::string directive,std::vector<std::string> vec,int countSpaces);
                static bool isValidPath(std::string location_name);
                static void checkAutoindexError(std::vector<std::string> autoindex);
                static void checkMethodsError(std::vector<std::string> methods); 
                static void  checkLocationName(std::vector<std::string> location_name);   
                static void checkAllowedUpload(std::vector<std::string> allowedApload);
                static void checkIndexError(std::vector<std::string> index);
                static void checkReturnError(std::vector<std::string> returnDirective);
                static void checkRootError(std::vector<std::string> root);
                static void checkUpload(std::vector<std::string> upload);
                ~Location();
                class autoindexError:public std::exception 
                {
                        const char* what() const throw()
                        {
                                return "autoindex error\n";
                        }
                };
                class httpMethodsError:public std::exception 
                {
                        const char* what() const throw()
                        {
                                return "http methods error\n";
                        }
                };
                class indexError:public std::exception 
                {
                        const char* what() const throw()
                        {
                                return "index error\n";
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
                                return "directives name error \n";
                        }
                };
                class rootError:public std::exception 
                {
                        const char* what() const throw()
                        {
                                return "directives name error \n";
                        }
                };
                class location_nameError:public std::exception 
                {
                        const char* what() const throw()
                        {
                                return "location_name error \n";
                        }
                };
                class uploadError:public std::exception 
                {
                        const char* what() const throw()
                        {
                                return "upload error\n";
                        }
                };
                class returnError:public std::exception 
                {
                        const char* what() const throw()
                        {
                                return "return error \n";
                        }
                };
};


