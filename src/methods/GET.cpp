/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   GET.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lsadiq <lsadiq@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/02 13:09:22 by lsadiq            #+#    #+#             */
/*   Updated: 2024/02/07 20:56:57 by lsadiq           ###   ########.fr       */
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
    this->root = "./User/lsadiq/Desktop/";
    this->target_url = "SpongeBob.mp4";
    this->location_name = "/User/";
    this->uploade = "off";
    this->return_ = "200";
    this->status_code = 200;
}
void    location::fillMime()
{
    std::ifstream file;
    location loc;
    file.open("mime.txt");
    if (file.bad())
    {
        std::cerr << "Error: couldn't open mime file" << std::endl;
        return;
    }
    for(;getline(file, loc.line, ',');)
    {
        try{
            getline(file, loc.name);
            _mime[loc.line] = loc.name;
        }
        catch(const std::exception &e){
            
            std::cerr<<"Couldn't fill the map"<<std::endl;
        }
    }
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
    if (idx != std::string::npos){
        extention = file.substr(idx + 1);
        extention =  _mime[extention];
    }
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
    // if (status_code != 200)
    //     handel_error();
    
    if(!allowedMethods())
        status_code = 405;
    //newlocation = url (remplace loc with root);
    newlocation = "./User/lsadiq/Desktop/SpongeBob.mp4";
    if(access(this->newlocation.c_str(), F_OK) == 0)
    {
        if(access(this->newlocation.c_str(), R_OK) == 0)
        {
            if(checkType(newlocation) == FILE)
            {
                file.open(newlocation.c_str(), std::ios::in | std::ios::out);
                if(file.bad()){
                    status_code = 403;
                    return ;
                }
                else
                {
                    check_extention(newlocation);
                    content_type = extention;
                    status_code = 200;
                }
                file.close();
            }
            if (checkType(newlocation) == DIRECTORY)
            {
                if (newlocation[newlocation.length() - 1] != '/')
                {
                    status_code = 301;
                    newlocation = this->target_url + "/";
                    return;
                }
                // if (!index.empty() ) // check that F_OK and R_OK 
                // {
                //     status_code = 301;
                //     newlocation = this->target_url + index;
                //     return;
                // }
                // else if (autoIndexCheck()){
                    
                    
                // }
                // else
                //     status_code = 403;
                // return ;
            }
            if (checkType(newlocation) == NOT_FOUND)
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
        std::cout << "newlocation doesn't exist" << std::endl;
        status_code = 404;
    }
}