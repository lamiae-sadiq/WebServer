/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Location.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kel-baam <kel-baam@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/12 16:53:58 by kel-baam          #+#    #+#             */
/*   Updated: 2024/03/17 14:38:05 by kel-baam         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/Location.hpp"


Location::Location(){};

Location::~Location(){};

void Location::setLocationData(std::string key, std::vector<std::string> vec)
{
    if(locationData.count(key) >= 1)
        throw Exception("Error: directive is duplicated\n");
    if(key == "cgi")
        locationData[vec[0]].insert(locationData[vec[0]].end(),vec[1]);
    else
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

void Location::checkAutoindexError(std::vector<std::string> autoindex)
{
    if(autoindex.size() != 1)
        throw Exception("Error: autoindex is incorrect\n");
    if(autoindex[0] != ON && autoindex[0] != OFF)
        throw Exception("Error: autoindex is incorrect\n");
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
        throw Exception("Error: methods is incorrect\n");
    if(std::count(methods.begin(),methods.end(),POST_METHODE) > 1 || 
    std::count(methods.begin(),methods.end(),DELETE_METHODE) > 1 || 
    std::count(methods.begin(),methods.end(),GET_METHODE) > 1)
        throw Exception("Error: method is duplicated\n");
    for(size_t i = 0; i < methods.size();i++)
    {
        if(methods[i] != GET_METHODE && methods[i] != POST_METHODE && methods[i] != DELETE_METHODE)
            throw Exception("Error: methods is incorrect\n");
    }
}

void  Location::checkLocationName(std::vector<std::string> location_name)
{
    if(location_name.size() != 1 || location_name[0][0] != '/')
        throw Exception("Error: loaction name is incorrect\n");
}

void Location::checkUpload(std::vector<std::string> upload)
{
    if(upload.size() != 1)
        throw Exception("Error: uplad path is incorrect\n");
}

void Location::checkAllowedUpload(std::vector<std::string> allowedApload)
{
    if(allowedApload.size() != 1 || (allowedApload[0] != ON && allowedApload[0] != OFF))
        throw Exception("Error: allowedUpload is incorrect\n");
}

void Location::checkIndexError(std::vector<std::string> index)
{
    if(index.size() != 1 || Utils::isWhitSpaces(index[0][0]))
        throw Exception("Error: index is incorrect\n");
}

void Location::checkReturnError(std::vector<std::string> returnDirective)
{
    long long int returnCode = Utils::stringToLongLong(returnDirective[0]);
    if(!Utils::checkOverflowError(returnDirective[0],returnCode))
        throw Exception("Error: redirection is incorrect\n");
    if(returnDirective.size() != 2)
        throw Exception("Error: redirection is incorrect\n");
    if(returnCode != 301)
        throw Exception("Error: redirection is incorrect\n");
}

void Location::checkRootError(std::vector<std::string> root)
{
    size_t len = root[0].length() - 1;
    if(root.size() != 1 || root[0][0] != '/' || root[0][len]!= '/')
        throw Exception("Error: root is incorrect\n");
}

void Location::checkCgiError(std::vector<std::string> vec)
{
    if(vec.size() != 2)
        throw Exception("Error: cgi is incorrect\n");
}

void Location::checkLocationError(std::string directive,std::vector<std::string> vec,int countSpaces)
 {
    if(countSpaces != 2)
        throw Exception("Error: configuefile format is incorrect\n");
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
    else
        throw Exception("Error: directive name is incorrect\n");
}
