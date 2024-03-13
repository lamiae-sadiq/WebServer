/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Location.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kel-baam <kel-baam@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/12 16:53:58 by kel-baam          #+#    #+#             */
/*   Updated: 2024/03/12 16:09:31 by kel-baam         ###   ########.fr       */
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

void Location::checkMembershipInLocation(std::string directive)
{
    if(directive != "index" && directive !="autoindex" && directive != "http_methods" && directive != "return" &&
     directive!= "root" && directive != "location_name" && directive != "error_page" && directive != "upload" &&
      directive!= "allowedUpload" && directive != "cgi")
        throw directiveError();
}

void Location::checkAutoindexError(std::vector<std::string> autoindex)
{
    if(autoindex.size() != 1)
        throw autoindexError();
    if(autoindex[0] != ON && autoindex[0] != OFF)
        throw autoindexError();
}
bool Location::isValidLocation()
{
    if(locationData.count("location_name") == 1 && locationData.count("root") == 1)
        return true;
    return false;
}
void Location::checkMethodsError(std::vector<std::string> methods)
{
    if(methods.size() > 3 || methods.size() < 1)
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
    if(location_name.size() != 1 || location_name[0][0] != '/')
        throw location_nameError();
}

void Location::checkUpload(std::vector<std::string> upload)
{
    if(upload.size() != 1)
        throw uploadError();
}

void Location::checkAllowedUpload(std::vector<std::string> allowedApload)
{
    if(allowedApload.size() != 1 || (allowedApload[0] != ON && allowedApload[0] != OFF))
        throw uploadError();
}

void Location::checkIndexError(std::vector<std::string> index)
{
    if(index.size() != 1)
        throw indexError();
}

void Location::checkReturnError(std::vector<std::string> returnDirective)
{
    if(returnDirective.size() != 2)
        throw returnError();
    //check if the first  umber 301 and  another one the second one is a path maybe i should check for him too
}

void Location::checkRootError(std::vector<std::string> root)
{
    size_t len = root[0].length() - 1;
    if(root.size() != 1 || root[0][0] != '/' || root[0][len]!= '/')
        throw rootError();
}
//check errors here

void Location::checkCgiError(std::vector<std::string> vec)
{
    if(vec.size() != 2)
        throw cgiError();
}

void Location::checkLocationError(std::string directive,std::vector<std::string> vec,int countSpaces)
 {
    if(countSpaces != 2)
        throw ConfigueFileError();
    checkMembershipInLocation(directive);
    if(directive == "index")
        checkIndexError(vec);
    else if(directive =="location_name")
        checkLocationName(vec);
    else if(directive == "allowedUpload")
       checkAllowedUpload(vec);
    else if(directive == "upload")
        checkUpload(vec);
    else if(directive == "autoindex")
        checkAutoindexError(vec);
    else if(directive == "http_methods")
        checkMethodsError(vec);
    else if(directive == "return")
        checkReturnError(vec);
    else if(directive == "root")
        checkRootError(vec);
    else if(directive == "cgi")
        checkCgiError(vec);
    ///cgi errors
}
