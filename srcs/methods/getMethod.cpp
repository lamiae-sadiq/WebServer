/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   getMethod.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kel-baam <kel-baam@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/02 13:09:22 by lsadiq            #+#    #+#             */
/*   Updated: 2024/03/19 14:00:36 by kel-baam         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/response.hpp"
#include "../../includes/Request.hpp"
#include "../../includes/cgi.hpp"


void response::init()
{
    fd = -1;
    this->status_code = 200;
	this->targetUri = "";
    _cgiStarted = false;
    _cgiEnded = false;
    _isCgi = false;
    // _isCgi  = false;
    // close = true;
    // this->_cgiMap = request.location.cgi;
    flag = 0;
    this->ihex = 0;
    lastChunk = false;
    // _isStatus = false;
    dir = NULL;
    postDone = false;
    fileExtention();
}

response::response(Request& initRequest):request(initRequest)
{
	init();
}


std::map<std::string, std::string> response::_mime;
response::~response(){}
    


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
        case 408:
            return "Request Timeout";
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
        case 504:
            return "Gateway Timeout";
		default:
			return "Internal Server Error";
	}
}

void    response::methodGet()
{
    if (flag == 0)
    {
        if (!request.location.redirect_path.empty())
        {
            target_url = request.location.redirect_path;
            status_code = 301;
            return;
        }
        targetUri = request.location.root + request.getUrl().substr(request.location.location_name.size());
        // request.getUri() = targetUri;
        //  std::cout << "targetUri   " << targetUri << std::endl;
        if(!allowedMethods())
            status_code = 405;
        else if(access(this->targetUri.c_str(), F_OK) == 0)
        {
            if(access(this->targetUri.c_str(), R_OK) == 0)
            {
                if(checkType(targetUri) == FILE)
                {
                    check_extention(targetUri);
                    if(extention == "php" || extention == "py")
                    {
                        _isCgi = true;
                        handelCGI();
                        return;
                    }
                    else{
                        content_type = fileType;
                        flag = 2;
                        return;
                    }
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
                    else{
                        status_code = 403;
                    }
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
 