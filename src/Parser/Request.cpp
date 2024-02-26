/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kel-baam <kel-baam@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/20 17:00:45 by kel-baam          #+#    #+#             */
/*   Updated: 2024/02/24 17:51:06 by kel-baam         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/Request.hpp"


Request::Request()
{
	tmpBuff = "";
	countHeaders = 0;
	isDone = 0;
	isChunke = 1;
	status = 0;
};

loc Request::getLocation()
{
	return this->location;
}

void Request::setHeader(std::string &key,std::string &value)
{
	headers[key] = value;
}
int Request::getStatus()
{
	return status;
}

std::string Request::getHeader(std::string key)
{
	return headers[key];
}

std::string Request::getMethod()
{
	return method;
}

std::string Request::getUrl()
{
	return url;
}

std::string Request::getVersion()
{
	return version;
}

size_t Request::getContentLength()
{
	return contentLength;
}

// std::string Request::getContentType()
// {
// 	return contentType;
// }

void Request::setMethod(std::string initVar)
{
	method = initVar;
}

void Request::setUrl(std::string initVar)
{
	url = initVar;
}

void Request::setVersion(std::string initVar)
{
	version = initVar;
}

// void Request::setContentLength(size_t initVar)
// {
// 	contentLength = initVar;
// }

void Request::storeRequestLineInfo(std::vector<std::string> vec)
{
	// std::cout << vec[0] << std::endl;
	// if(vec.size() != 3)
	// 	throw headerError(); // maybe i should send you a notif to know what error you should display
	setMethod(vec[0]);
	// unimplemetnted method or bad request
	setUrl(vec[1]);
	// check if there is /
	setVersion(vec[2]);
	// check if HTTP/1.1
}

		
void Request::storeHostHeader(std::string line)
{
	Utils::skipSpaces(line);
	size_t index = line.find(":");
	std::string port;
	if(index != std::string::npos)
	{
		host = line.substr(0,index);
		port = line.substr(index+1);
	}
	else
		host = line;
}

void Request::storeRequest(std::string line)
{
	Utils::skipSpaces(line);
	size_t index = line.find(":");
	std::string key = line;
	if(!countHeaders)
	{
	 	storeRequestLineInfo(Utils::splitString(line,' '));
		countHeaders = 1;
		return;
	}
	if(index != std::string::npos)
	{
		key = line.substr(0,index);
		line = line.substr(index+1);
	}
	if(key == "Host")
		storeHostHeader(line);
	else if(key == "Content-Length")
		contentLength = atoi(line.c_str());
	// else if(key == )
	//check headers key : value
	//setHeader(key,line);
}

void Request::analyseHeaders(std::string buff)
{
	size_t index = 1;
	std::string requests;
	requests = tmpBuff + buff;
	
	while(index != std::string::npos)
	{
		index = requests.find("\n");
		if(index != std::string::npos)
		{
			storeRequest(requests.substr(0,index));
			requests = requests.substr(index + 1);
			if(requests == "\r\n")
		 	{
				tmpBuff = requests;
				status = 1;
				isDone = 1;	
			}
		}
		else if(!requests.empty())
		{
			tmpBuff = requests;
		}
		else
			tmpBuff = "";
	}
}

std::string Request::checkemptyData(std::vector<std::string> vec,int index)
{
	if(!vec.empty())
	{
		return vec[index];
	}
	return "";
}
void Request::storeLocation(Server &server, Location iniLocation)
{
	// error pages
	location.redirect_code = atoi(checkemptyData(iniLocation.getLocationData("return"),0).c_str());
	location.redirect_path = checkemptyData(iniLocation.getLocationData("return"),1);
	location.max_body_size = atoi(server.getServerData("client_max_body_size")[0].c_str());
	location.root =	checkemptyData(server.getServerData("root"),0);
	location.index =  checkemptyData(iniLocation.getLocationData("index"),0);
	location.auto_index = checkemptyData(iniLocation.getLocationData("autoindex"),0);
	location.upload = checkemptyData(iniLocation.getLocationData("upload"),0);
	location.location_name =  checkemptyData(iniLocation.getLocationData("location_name"),0);
	location.method =  iniLocation.getLocationData("http_methods");
	if(!(iniLocation.getLocationData("allowedUpload").empty()) &&  iniLocation.getLocationData("allowedUpload")[0] == "on")
		location.allowedUpload = true;
	else
		location.allowedUpload = false;
}

void  Request::printREquest()
{
	std::cout << "max_body_size ======>>>>" <<  location.max_body_size <<"\n";
	std::cout << "root ======>>>>" <<  location.root<<"\n";
	std::cout << "index ======>>>>" <<  location.index<<"\n";
	std::cout << "auto_index ======>>>>" <<  location.auto_index<<"\n";
	std::cout << "upload ======>>>>" <<  location.upload<<"\n";
	std::cout << "location_name ======>>>>" <<  location.location_name<<"\n";
	std::cout << "allowedUpload ======>>>>" <<  location.allowedUpload<<"\n";
	std::cout << "redirect_path ======>>>>" << location.redirect_path<<"\n";
	for(size_t i = 0;i < location.method.size();i++)
	{
		std::cout << "methods ======>>>>" <<  location.method[i] << "\n";
	}
}
// void Request::setContentType(std::string initVar)
// {
// 	content_Type = initVar;
// }
int Request::matchLocation(std::string host,Server server)
{
	if(host == server.getServerData("server_name")[0])
	{
		for(size_t j = 0; j < server.getLocations().size();j++)
		{
			if(server.getLocations()[j].getLocationData("location_name")[0] == url)
			{
				storeLocation(server,server.getLocations()[j]);
			
				break;
			}
		}
		return 1;
	}
	return 0;
}
void Request::matchServer()
{
	for(size_t i =0; i<servers.size();i++)
	{
		if(matchLocation(host, servers[i]))
			break;
	}
}

int Request::parseHeaders(std::string buff,std::vector<Server> initServers)
{
	std::cout << "kkkkkk\n";
	if(!status)
	{
		analyseHeaders(buff);
		servers = initServers;
		
		if(isDone)
		{
			status = 1;
			matchServer();
			return 1;
		}
	}
	else
		return 1;
	return 0;
}