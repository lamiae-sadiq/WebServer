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

#include "../includes/Request.hpp"

void Request::setHeader(std::string &key,std::string &value)
{
	headers[key] = value;
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

std::string Request::getContentLength()
{
	isChunke =  0;//false 
	return contentLength;
}

std::string Request::getContentType()
{
	return contentType;
}

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

void Request::setContentLength(std::string initVar)
{
	contentLength = initVar;
}

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
	//check headers key : value
	//setHeader(key,line);
}
void Request::analyseHeaders(std::string buff)
{
	int index = 1;
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

std::string Request::checkemptyData(std::vector<std::string> vec)
{
	if(!vec.empty())
	{
		return vec[0];
	}
	return "";
}
void Request::storeLocation(Server &server, Location iniLocation)
{
	//return
	location.max_body_size = atoi(server.getServerData("client_max_body_size")[0].c_str());
	location.root =	checkemptyData(server.getServerData("root"));
	location.index =  checkemptyData(iniLocation.getLocationData("index"));
	location.auto_index = checkemptyData(iniLocation.getLocationData("autoindex"));
	location.upload = checkemptyData(iniLocation.getLocationData("upload"));
	location.location_name =  checkemptyData(iniLocation.getLocationData("location_name"));
	location.method =  iniLocation.getLocationData("http_methods");
	if(iniLocation.getLocationData("allowedUpload").empty() &&  iniLocation.getLocationData("allowedUpload")[0]== "on")
		location.allowedUpload = true;
	else
		location.allowedUpload = false;
}

void Request::setContentType(std::string initVar)
{
	contentType = initVar;
}
int Request::matchLocation(std::string host,Server server)
{
	if(host == server.getServerData("server_name")[0])
	{
		for(size_t j = 0; j < server.getLocations().size();j++)
		{
			if(server.getLocations()[j].getLocationData("location_name")[0] == url)
			{
				storeLocation(server,server.getLocations()[j]);
				// std::cout << "======>>>>" <<  location.max_body_size <<"\n";
				// std::cout << "======>>>>" <<  location.root<<"\n";
				// std::cout << "======>>>>" <<  location.index<<"\n";
				// std::cout << "======>>>>" <<  location.auto_index<<"\n";
				// std::cout << "======>>>>" <<  location.upload<<"\n";
				// std::cout << "======>>>>" <<  location.location_name<<"\n";
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
	analyseHeaders(buff);
	servers = initServers;
	
		status = 1;
	if(isDone)
	{
		matchServer();
		return 1;
	}
	return 1;
}