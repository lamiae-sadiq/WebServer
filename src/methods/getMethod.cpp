/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   getMethod.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lsadiq <lsadiq@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/02 13:09:22 by lsadiq            #+#    #+#             */
/*   Updated: 2024/02/24 17:48:50 by lsadiq           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/response.hpp"
#include "../../includes/Request.hpp"


// void response::init()
// {
//     request.location.redirect_code = 200,
//     request.location.index = "",
//     request.location.root = "/nfs/homes/kel-baam/Desktop/newV";
//     request.location.auto_index = "on",
//     request.location.upload = "/upload",
//     request.location.allowedUpload = true,
//     request.location.location_name = "/",
//     request.location.method.push_back("POST");
//     request.location.method.push_back("GET");
//     request.location.method.push_back("DELETE");

//     this->target_url = "/webCuddler/body";
//     fd = -1;
//     this->status_code = 200;
//     this->request.contentLength = 200000000000;
//     close = true;
// 	this->targetUri = "";
//     this->uplod_type = "Chunk";
//     flag = 0;
//     this->ihex = 0;
//     dir = NULL;
// }

std::map<std::string, std::string> response::_mime;
response::~response(){}

// void response::setRequest(Request initReq)
// {
// 	std::cout << "init REq\n";
//     (void)initReq;
// 	// request = initReq;
//     init();
			
// }
response::response(){
    request.location.redirect_code = 200,
    request.location.index = "",
    request.location.root = "/nfs/homes/kel-baam/Desktop/newV";
    request.location.auto_index = "on",
    request.location.upload = "/upload",
    request.location.allowedUpload = true,
    request.location.location_name = "/",
    request.location.method.push_back("POST");
    request.location.method.push_back("GET");
    request.location.method.push_back("DELETE");

    this->target_url = "/webCuddler/body";
    fd = -1;
    this->status_code = 200;
    this->request.contentLength = 200000000000;
    close = true;
	this->targetUri = "";
    this->uplod_type = "Chunk";
    flag = 0;
    this->ihex = 0;
    dir = NULL;
}

void response::listDirectories()
{
    std::string html;
    if(flag == 6){
        std::string header = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\nTransfer-Encoding: chunked\r\n\r\n";
        send(fd, header.c_str(), header.length(), 0);
        flag = 7;
    }
    else if(flag == 7){
        html.append("<html>\n<head>\n<title>Index of ");
        html.append(target_url); 
        html.append("</title>\n</head>\n<body>\n<h1>Index of ");
        html.append(target_url); 
        html.append("</h1>\n<hr>\n<ul>\n");
        std::stringstream ss;
        ss << std::hex << html.length() << "\r\n";
        send(fd, ss.str().c_str(), ss.str().length(), 0);
        html.append("\r\n");
        send(fd, html.c_str(), html.length(), 0);
        if (!(dir = opendir(targetUri.c_str())))
        {
            std::cout << "Error: failed to open directory" << std::endl;
            status_code = 500;
            return;
        }
        flag = 8;
    }
    else if(flag == 8){
        struct dirent* entry;
        if (!(entry = readdir(dir)))
        {
            flag = 9;
            return;
        }
        std::string fileName = entry->d_name;
        if (fileName[0] == '.')
            return;
        html += "<li><a href=\"" + fileName + "\">" + fileName + "</a></li>\n";
        std::stringstream ss;
        ss << std::hex << html.length() << "\r\n";
        send(fd, ss.str().c_str(), ss.str().length(), 0);
        html.append("\r\n");
        send(fd, html.c_str(), html.length(), 0);
    }
    else if(flag == 9){
        closedir(dir);
        dir = NULL;
        html = "</ul>\n<hr>\n</body>\n</html>\n";
        std::stringstream ss;
        ss << std::hex << html.length() << "\r\n";
        send(fd, ss.str().c_str(), ss.str().length(), 0);
        html.append("\r\n");
        send(fd, html.c_str(), html.length(), 0);
        flag = 10;
    }
    else if(flag == 10){
        send(fd, "0\r\n\r\n", 5, 0);
        close = 0;
    }
}

void response::handel_error() {
    if (status_code == 404)
        send(fd, "HTTP/1.1 404 not found\r\nContent-Type: text/html\r\nContent-length:  19\r\n\r\n<h1>not found</h1>\n", 92, 0);
    else if (status_code == 403)
        send(fd, "HTTP/1.1 403 forbeiden\r\nContent-Type: text/html\r\nContent-length:  19\r\n\r\n<h1>forbeiden</h1>\n", 92, 0);
    else if (status_code == 301)
        send(fd, "HTTP/1.1 301 moved permanently\r\nContent-Type: text/html\r\nContent-length:  26\r\n\r\n<h1>moved permanently</h1>\n", 108, 0);
    else if (status_code == 405)
        send(fd, "HTTP/1.1 405 method not allowed\r\nContent-Type: text/html\r\nContent-length:  27\r\n\r\n<h1>method not allowed</h1>\n", 110, 0);
    else if (status_code == 500)
        send(fd, "HTTP/1.1 500 server do\r\nContent-Type: text/html\r\nContent-length:  19\r\n\r\n<h1>server do</h1>\n", 92, 0);
    else if (status_code == 501)
        send(fd, "HTTP/1.1 501 not implemented\r\nContent-Type: text/html\r\nContent-length:  24\r\n\r\n<h1>not implemented</h1>\n", 104, 0);
    else
        send(fd, "HTTP/1.1 500 server do\r\nContent-Type: text/html\r\nContent-length:  19\r\n\r\n<h1>server do</h1>\n", 92, 0);
    close = 0;
}



void    response::methodGet()
{
    request.printREquest();
    if (status_code != 200)
    {
        handel_error();
        return;
    }
    if (flag == 0)
    {
        targetUri = request.location.root + target_url.substr(request.location.location_name.size());
        targetUri = "/nfs/homes/kel-baam/Desktop/newV/vd.mp4";
        if(!allowedMethods())
            status_code = 405;
        else if(access(this->targetUri.c_str(), F_OK) == 0)
        {
            if(access(this->targetUri.c_str(), R_OK) == 0)
            {
                if(checkType(targetUri) == FILE)
                {
                    check_extention(targetUri);
                    content_type = fileType;
                    flag = 2;
                    return;
                }
                if (checkType(targetUri) == DIRECTORY)
                {
                    if (targetUri[targetUri.length() - 1] != '/')
                    {
                        status_code = 301;
                        target_url.append("/");
                        return;
                    }
                    if (!request.location.index.empty() && (access((targetUri + request.location.index).c_str(), F_OK | R_OK) == 0)) 
                    {
                        status_code = 301;
                        target_url.append(request.location.index);
                        // std::cout << targetUri << std::endl;
                        return;
                    }
                    else if (autoIndexCheck())
                    {
                        content_type = "text/html";
                        flag = 6;
                        return ;
                    }
                    else
                        status_code = 403;
                    return ;
                }
                if (checkType(targetUri) == NOT_FOUND)
                {
                    std::cout << " not found "<< std::endl;
                    status_code = 403;
                }
                else
                    status_code = 403;
            }
        }
        else
        {
            std::cout << "targetUri doesn't exist" << std::endl;
            status_code = 404;
        }

    }
    else if (flag >= 2 && flag <= 4)
        sendData();
    else if (flag >= 6 && flag <= 10)
        listDirectories();
}