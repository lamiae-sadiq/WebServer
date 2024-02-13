/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   GET.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lsadiq <lsadiq@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/02 13:09:22 by lsadiq            #+#    #+#             */
/*   Updated: 2024/02/13 21:12:00 by lsadiq           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/webs.hpp"
#include <dirent.h>

std::map<std::string, std::string> location::_mime;

location::~location(){}

location::location()
{
    this->newlocation = "";
    this->index = "index.html";
    this->auto_index = "off";
    this->method.push_back("GET");
    this->method.push_back("POST");
    this->root = "/home/weirdo/Desktop/";
    this->target_url = "SpongeBob.mp4";
    this->location_name = "/nfs/homes/lsadiq/Desktop/webCuddler/ecran.jpg";
    this->uploade = "off";
    this->return_ = "200";
	this->newlocation = "/nfs/homes/lsadiq/Desktop/webCuddler/ecran.jpg";
    this->status_code = 200;
    fd = -1;
    close = true;
    flag = 0;
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
    for(;getline(file, loc.line, ';');)
    {
        try{
            getline(file, loc.name);
            _mime[loc.line] = loc.name;
        }
        catch(const std::exception &e){
            std::cerr<<"Couldn't fill the map"<<std::endl;
        }
    }
    file.close();
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

void location::listDirectories()
{
    location loc;
    std::string html = "<html>\n<head>\n<title>Index of ";
    html.append(loc.newlocation); 
    html.append("</title>\n</head>\n<body>\n<h1>Index of ");
    html.append(loc.newlocation); 
    html.append("</h1>\n<hr>\n<ul>\n");

    DIR* dir;
    struct dirent* entry;
    if ((dir = opendir(loc.newlocation.c_str())))
    {
        while ((entry = readdir(dir)))
        {
            std::string fileName = entry->d_name;
            if (fileName != "." && fileName != ".."  && fileName[0] != '.')
            {
                html += "<li><a href=\"" + fileName + "\">" + fileName + "</a></li>\n";
            }
        }
        closedir(dir);
    }

    html += "</ul>\n<hr>\n</body>\n</html>\n";
    // std::cout << html << std::endl;
    //generated HTML, like writing it to a file or sending it as a response.
    std::ofstream outputFile("index.html");
    newlocation = "index.html";
    outputFile << html;
    outputFile.close();
}

void location::handel_error() {
    if (status_code == 404)
        send(fd, "HTTP/1.1 404 not found\r\nContent-Type: text/html\r\nContent-length:  19\r\n\r\n<h1>not found</h1>\n", 91, 0);
    else if (status_code == 403)
        send(fd, "HTTP/1.1 403 forbeiden\r\nContent-Type: text/html\r\nContent-length:  19\r\n\r\n<h1>forbeiden</h1>\n", 91, 0);
    else if (status_code == 301)
        send(fd, "HTTP/1.1 301 moved permanently\r\nContent-Type: text/html\r\nContent-length:  19\r\n\r\n<h1>moved permanently</h1>\n", 91, 0);
    else if (status_code == 405)
        send(fd, "HTTP/1.1 405 method not allowed\r\nContent-Type: text/html\r\nContent-length:  19\r\n\r\n<h1>method not allowed</h1>\n", 91, 0);
    else if (status_code == 500)
        send(fd, "HTTP/1.1 500 server do\r\nContent-Type: text/html\r\nContent-length:  19\r\n\r\n<h1>server do</h1>\n", 91, 0);
    else if (status_code == 501)
        send(fd, "HTTP/1.1 501 not implemented\r\nContent-Type: text/html\r\nContent-length:  19\r\n\r\n<h1>not implemented</h1>\n", 91, 0);
    else
        send(fd, "HTTP/1.1 500 server do\r\nContent-Type: text/html\r\nContent-length:  19\r\n\r\n<h1>server do</h1>\n", 91, 0);
    close = 0;
}

void    location::sendData()
{
    if (flag == 2)
        {
            std::cout << flag << std::endl;
            ifile.open(newlocation);
            if (!ifile.is_open())
                throw std::runtime_error("Error: failed to open video ifile");
            std::string resHeader = "HTTP/1.1 " + std::to_string(status_code) + " OK\r\n";
                resHeader += "Content-Type: ";
                resHeader += content_type;
                resHeader +="\r\n";
                resHeader += "Transfer-Encoding: chunked\r\n";
                resHeader += "\r\n";
            send(fd, resHeader.c_str(), resHeader.length(), 0);
            flag = 3;
        }
        else if (flag == 3)
        {
            std::cout << flag << std::endl;
            const int chunkSize = 1024;
            char buffer[chunkSize];
            ifile.read(buffer, chunkSize);
            int bytesRead = ifile.gcount();
            std::stringstream ss;
            ss << std::hex << bytesRead;
            std::string chunkSizeHex = ss.str();
            std::string chunkHeader = chunkSizeHex + "\r\n";
            send(fd, chunkHeader.c_str(), chunkHeader.length(), 0);
            send(fd, buffer, bytesRead, 0);
            send(fd, "\r\n", 2, 0);
            if (ifile.eof())
                flag = 4;
        }
        else if (flag == 4) {
            std::cout << flag << std::endl;
            send(fd, "0\r\n\r\n", 5, 0);
            ifile.close();
            close = 0;
        }
}

void    location::methodGet()
{
   
    if (status_code != 200)
    {
        handel_error();
        return;
    }
    if (flag == 0)
    {
        if(!allowedMethods())
            status_code = 405;
        else if(access(this->newlocation.c_str(), F_OK) == 0)
        {
            if(access(this->newlocation.c_str(), R_OK) == 0)
            {
                if(checkType(newlocation) == FILE)
                {
                    check_extention(newlocation);
                    content_type = extention;
                    flag = 2;
                    return;
                }
                if (checkType(newlocation) == DIRECTORY)
                {
                    if (newlocation[newlocation.length() - 1] != '/')
                    {
                        status_code = 301;
                        newlocation = this->target_url + "/";
                    }
                    listDirectories();
                    content_type = "text/html";
                    flag = 2;
                    return ;
                }
            }
                //     if (!index.empty() && (access(index.c_str(), F_OK | R_OK) == 0)) 
                //     {
                //         status_code = 301;
                //         newlocation = this->target_url + index;
                //         return;
                //     }
                    // else if (autoIndexCheck()){

                    // }
                    // else
                    //     status_code = 403;
                    // return ;
                // }
            //     if (checkType(newlocation) == NOT_FOUND)
            //     {
            //         std::cout << " not found "<< std::endl;
            //         status_code = 403;
            //     }
            // }
            // else
            //     status_code = 403;
        }
        else
        {
            std::cout << "newlocation doesn't exist" << std::endl;
            status_code = 404;
        }

    }
    else if (flag >= 2 && flag <= 4)
        sendData();
}