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

#include "../../includes/Location.hpp"


Location::Location(){};

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
    // std::cout << locationData.size() << std::endl;
    // if(!locationData["autoindex"].empty())
    //     std::cout << "      autoindex "<< locationData["autoindex"][0] << std::endl;
    // if(!locationData["http_methods"].empty())
    //     std::cout << "      http_methods "<<  locationData["http_methods"][0] << std::endl;
    // if(!locationData["index"].empty())
    //     std::cout << "      index "<< locationData["index"][0] << std::endl;
    // if(!locationData["return"].empty())
    //     std::cout << "      return " << locationData["return"][0] << std::endl;
    // if(!locationData["location_name"].empty())
    //     std::cout << "      location_name " << locationData["location_name"][0] << std::endl;
    // if(!locationData["root"].empty())
    //     std::cout << "      root " << locationData["root"][0] << std::endl;
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
    if(autoindex[0] != ON && autoindex[0] != OFF)
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

void  Location::checkLocationName(std::vector<std::string> location_name)
{
    if(location_name.size() > 1 || location_name[0][0] != '/')
        throw location_nameError();
    //should i check that location_name or not in this step 
}

void Location::checkUpload(std::vector<std::string> allowedApload)
{
    if(allowedApload.size() > 1 || (allowedApload[0] != ON && allowedApload[0] != OFF))
        throw uploadError();
}

void Location::checkIndexError(std::vector<std::string> index)
{
    //check if is just string or extension???
    // check extension

    if(index.size() > 1)
        throw indexError();
}
void Location::checkReturnError(std::vector<std::string> returnDirective)
{
    if(returnDirective.size() > 2)
        throw returnError();
    //check if the first  umber 301 and  another one the second one is a path maybe i should check for him too
}

void Location::checkRootError(std::vector<std::string> root)
{
    if(root.size() > 1 || root[0][0] != '/')
        throw rootError();
    //check here / 
}

void Location::checkLocationError(std::string directive,std::vector<std::string> vec,int countSpaces)
 {
    if(countSpaces != 2)
        throw ConfigueFileError();
    if(directive == "index")
        checkIndexError(vec);
    if(directive =="location_name")
        checkLocationName(vec);
    if(directive == "allowedUpload")
        checkUpload(vec);
    else if(directive == "autoindex")
        checkAutoindexError(vec);
    else if(directive == "http_methods")
        checkMethodsError(vec);
    else if(directive == "return")
        checkReturnError(vec);
    else if(directive == "root")
        checkRootError(vec);
    //error pages
    // checkMembershipInLocation(directive);
    // index error not now
    //i store return but whitout spliting vec ==> 301 wwww.fwcebook.com
}
