/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Location.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kel-baam <kel-baam@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/12 16:53:58 by kel-baam          #+#    #+#             */
/*   Updated: 2024/02/24 16:05:43 by kel-baam         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Location.hpp"


Location::Location
(
){};
Location::~Location(){};
void Location::setLocationData(std::string key, std::vector<std::string> vec)
{
    locationData[key].insert(locationData[key].end(),vec.begin(),vec.end());
}

std::vector<std::string> Location::getLocationData(std::string key)
{
    std::vector<std::string> tmp;
    std::map<std::string,std::vector<std::string> >::iterator it;
    it = locationData.find(key);
    if(it!= locationData.end())
        tmp = locationData[key];
    return tmp;
}

void Location::printData()
{
    
    std::cout << locationData.size() << std::endl;
    if(!locationData["autoindex"].empty())
        std::cout << "      autoindex "<< locationData["autoindex"][0] << std::endl;
    if(!locationData["http_methods"].empty())
        std::cout << "      http_methods "<<  locationData["http_methods"][0] << std::endl;
    if(!locationData["index"].empty())
        std::cout << "      index "<< locationData["index"][0] << std::endl;
    if(!locationData["return"].empty())
        std::cout << "      return " << locationData["return"][0] << std::endl;
    if(!locationData["location_name"].empty())
        std::cout << "      location_name " << locationData["location_name"][0] << std::endl;
    if(!locationData["upload"].empty())
        std::cout << "      upload " << locationData["upload"][0] << std::endl;
}

void Location::checkMembershipInLocation(std::string directive)
{
    if(directive != "index" && directive !="autoindex" && directive != "http_methods" && directive != "return" &&
     directive!= "root" && directive != "location_name" && directive != "error_page" && directive != "upload" && directive!= "allowedUpload")
        throw directiveError();
}

void Location::checkAutoindexError(std::vector<std::string> autoindex)
{
    if(autoindex.size() > 1)
        throw autoindexError();
    if(autoindex[0] != "on" && autoindex[0] != "off")
        throw autoindexError();
}

void Location::checkMethodsError(std::vector<std::string> methods)
{
    if(methods.size() > 3)
        throw  httpMethodsError();
    if(std::count(methods.begin(),methods.end(),POST_METHODE) > 1 || 
    std::count(methods.begin(),methods.end(),DELETE_METHODE) > 1 || 
    std::count(methods.begin(),methods.end(),GET_METHODE) > 1)
            throw  httpMethodsError();

    for(size_t i = 0; i < methods.size();i++)
    {
        if(methods[i] != GET_METHODE && methods[i] != POST_METHODE && methods[i] != DELETE_METHODE)
            throw  httpMethodsError();
    }
}

void  Location::checkPathError(std::vector<std::string> location_name)
{
    if(location_name.size() > 1 || location_name[0][0] != '/')
        throw location_nameError();
    //should i check that location_name or not in this step 
}
// void Location::checkUpload(std::vector<std::string> allowedApload)
// {
    
//     if(allowedApload.size() > 1 || (allowedApload[0]== "on" && allowedApload[0]=="off"))
//         throw ConfigueFileError();
//     //check this error
// }
void Location::checkLocationError(std::string directive,std::vector<std::string> vec,int countSpaces)
 {
    //std::cout << directive << "====>" << vec[0] <<std::endl;
    std::cout << directive << std::endl;
    if(countSpaces != 2)
        throw ConfigueFileError();
    // if(directive == "allowedUpload")
    //     checkUpload(vec);
    // checkMembershipInLocation(directive);
    // index error not now
    //i store return but whitout spliting vec ==> 301 wwww.fwcebook.com
    if(directive =="location_name")
        checkPathError(vec);
    else if(directive == "autoindex")
        checkAutoindexError(vec);
    else if(directive == "http_methods")
        checkMethodsError(vec);
}
