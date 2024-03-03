/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   getMethod.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lsadiq <lsadiq@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/02 13:09:22 by lsadiq            #+#    #+#             */
/*   Updated: 2024/03/02 17:21:56 by lsadiq           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/response.hpp"
#include "../../includes/Request.hpp"


void response::init()
{
    // request.location.redirect_code = 200,
    // request.location.index = "",
    // request.location.root = "/nfs/homes/lsadiq/Desktop/web/srcs";
    // request.location.auto_index = "on",
    // request.location.upload = "/upload",
    // request.location.allowedUpload = true,
    // request.location.location_name = "/",
    // request.location.method.push_back("POST");
    // request.location.method.push_back("GET");
    // request.location.method.push_back("DELETE");

    // this->target_url = "/webCuddler/body";
    fd = -1;
    this->status_code = 200;
    // this->request.contentLength = 200000000000;
    close = true;
	this->targetUri = "";
    // this->uplod_type = "length";
    flag = 0;
    this->ihex = 0;
    lastChunk = false;
    dir = NULL;
}

template <typename T>
std::string to_string(T value) {
    std::ostringstream os;
    os << value;
    return os.str();
}

response::response(Request& initRequest):request(initRequest)
{
	init();
}

std::map<std::string, std::string> response::_mime;
response::~response(){}



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
        // close = 0;
    }
}

const std::string	response::setStatus(int status)
{
    switch (status)
	{
		case 200:
			return "OK";
		case 201:
			return "Created";
		case 204:
			return "No Content";
		case 301:
			return "Moved Permanently";
		case 403:
			return "Forbidden";
		case 404:
			return "Not Found";
		case 405:
			return "Method Not Allowed";
		case 500:
			return "Internal Server Error";
		case 501:
			return "Not Implemented";
        case 400:
            return "Bad request ";
        case 505:
            return "HTTP Version Not Supported";
		default:
			return "Internal Server Error";
	}
}

const std::string response::ErrorPage() {
    return ("<html lang=\"en\">\n"
            "<head>\n"
            "<meta charset=\"UTF-8\">\n"
            "<meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">\n"
            "<title>WebServe</title>\n"
            "<style>\n"
            "body {\n"
            "    margin: 0;\n"
            "    padding: 0;\n"
            "    height: 90vh;\n"
            "    background-image: url('https://64.media.tumblr.com/da82730a16d6b909da5d35044a4bafd1/tumblr_mr22e5xg0X1qbaxlqo1_400.gifv');\n"
            "    background-size: 2200px 1050px;\n"
            "    background-repeat: no-repeat;\n"
            "    background-attachment: fixed;\n"
            "    display: flex;\n"
            "    justify-content: center;\n"
            "    align-items: center;\n"
            "}\n"
            ".message {\n"
            "    font-size: 100px;\n"
            "    font-family: Fancopltasy, sans-serif;\n"
            "    color: hsl(217, 46%, 37%);\n"
            "    text-align: center;\n"
            "    text-shadow: 2px 2px 4px #000000;\n"
            "    font-weight: bold;\n"
            "}\n"
            "</style>\n"
            "</head>\n"
            "<body>\n"
            "<div class=\"message\">" + to_string(status_code) +"</span> "+ setStatus(status_code) +"</div>\n"
            "</body>\n"
            "</html>\n");
}

std::string response::getErrorPage() {
    if (serv.getErrorPage().find(status_code) != serv.getErrorPage().end()) {
        // std::cout << serv.getErrorPage()[status_code] << std::endl;
        return serv.getErrorPage()[status_code];
        // std::string headererr = "HTTP/1.1 " + to_string(status_code) + " " + setStatus(status_code);
        // headererr.append("\r\nContent-Type: text/html\r\nContent-length:  ");
        // headererr.append(to_string(serv.getErrorPage()[statusCode].length()));
        // headererr += "\r\n\r\n";
        // headererr.append(serv.getErrorPage()[statusCode]);
    }
    return "404";
}

void response::sendErrorPage()
{
    targetUri = serv.getErrorPage()[status_code];
    if(access(targetUri.c_str(), R_OK | W_OK) == 0)
    {
        flag = 2;
        sendData();
    }
    else
        ErrorHeader();
}

void response::ErrorHeader()
{
    std::cout << "check" << status_code <<"\n";
    const std::string errorPage = ErrorPage();
        std::string header = "HTTP/1.1 " + to_string(status_code) + " " + setStatus(status_code);
        header.append("\r\nContent-Type: text/html\r\nContent-length:  ");
		header.append(to_string(errorPage.length()));
        header += "\r\n\r\n";
        header.append(errorPage);
        send(fd, header.c_str(), header.length(), 0);
	flag = 30;
}

void response::handel_error() {
    switch(status_code){
		case 301: ErrorHeader();
        case 400: ErrorHeader();
		case 201: ErrorHeader();
		case 403: ErrorHeader();
		case 404: ErrorHeader();
		case 405: ErrorHeader();
		case 500: ErrorHeader();
		case 501: ErrorHeader();
		case 204: ErrorHeader();
    }
}


void    response::methodGet()
{
    if (flag == 0)
    { 
        if (status_code != 200)
        {
            handel_error();
            return;
        }
        targetUri = request.location.root + request.getUrl().substr(request.location.location_name.size());
        std::cout << "targetUri   " << targetUri << std::endl;
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