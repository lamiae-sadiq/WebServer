/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   GET.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lsadiq <lsadiq@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/02 13:09:22 by lsadiq            #+#    #+#             */
/*   Updated: 2024/02/06 19:31:04 by lsadiq           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/webs.hpp"
#include <dirent.h>

location::~location(){}

location::location()
{
    this->newlocation = "";
    this->index = "index.html";
    this->auto_index = "off";
    this->method.push_back("GET");
    this->method.push_back("POST");
    this->root = "./Desktop/";
    this->target_url = "./Desktop/WebServer";
    this->location_name = "/";
    this->uploade = "off";
    this->return_ = "200";
}

int checkType(std::string path)
{
    std::fstream file(path.c_str());
    if (file.is_open())
    {
        file.close();
        return FILE;
    }
    DIR *type = opendir(path.c_str());
    if(type)
    {
        closedir(type);
        return DIRECTORY;
    }
	else
		return NOT_FOUND;
}

void location::check_extention(std::string file)
{
    
    std::string::size_type idx;
    idx = file.rfind('.');
    if (idx != std::string::npos)
        extention = file.substr(idx + 1);
    else
        extention = "text/plain";
}

bool    location::autoIndexCheck(){
    if (auto_index == "on")
        return true;
    return false;
}

bool    location::allowedMethods()
{
    std::vector<std::string>::iterator it = this->method.begin();
    for(;it != this->method.end(); it++){
        if(*it == "GET" || *it == "POST" || *it == "DELETE")
            return true;
    }
    return false;
}

void    location::methodGet()
{
    if(!allowedMethods())
        status_code = 405;
    if(access(this->target_url.c_str(), F_OK) == 0)
    {
        if(access(this->target_url.c_str(), R_OK) == 0)
        {
            std::cout << "target_url exists" << std::endl;
            if(checkType(target_url) == FILE)
            {
                file.open(target_url.c_str(), std::ios::in | std::ios::out);
                if(file.bad()){
                    status_code = 403;
                    return ;
                }
                else
                {
                    check_extention(target_url);
                    content_type = extention;
                    status_code = 200;
                }
            }
            if (checkType(target_url) == DIRECTORY)
            {
                if (target_url[target_url.length() - 1] != '/')
                {
                    status_code = 301;
                    newlocation = this->target_url + "/";
                    std::cout<< newlocation << std::endl;
                    std::cout<< target_url << std::endl;
                    return ;
                }
                status_code = 301;
                return ;
            }
            if (checkType(target_url) == NOT_FOUND)
            {
                std::cout << " not found "<< std::endl;
                status_code = 403;
            }
        }
        else
            status_code = 403;
    }
    else
    {
        std::cout << "target_url doesn't exist" << std::endl;
        status_code = 404;
    }
}