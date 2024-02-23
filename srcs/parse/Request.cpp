/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kel-baam <kel-baam@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/20 17:00:45 by kel-baam          #+#    #+#             */
/*   Updated: 2024/02/23 13:40:58 by kel-baam         ###   ########.fr       */
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
void Request::analyseRequest(char *buff, int &completeRead)
{
	int index = 1;
	std::string requests ,str = buff;
	requests = tmpBuff + buff;
	
	while(index != std::string::npos)
	{
		index = requests.find("\n");
		if(index != std::string::npos)
		{
			std::cout << requests.substr(0,index) << std::endl;
			storeRequest(requests.substr(0,index));
			requests = requests.substr(index+1);
			std::cout << requests << std::endl;
			if(requests == "\r\n")
		 	{
				std::cout << requests << std::endl;
				tmpBuff = requests;
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

void Request::readReqeust(int &fd,int &completeRead)
{
	int byt;
	char buff[1024];

	std::cout << "reading from fd==>" << fd << "\n";
	byt = read(fd,buff,1024);
	if(byt < 0)
	{
		close(fd);
		throw readError();
	}
	buff[byt] = '\0';
	analyseRequest(buff,completeRead);
}

void Request::setContentType(std::string initVar)
{
	contentType = initVar;
}
void Request::matchLocation(std::string host,Server server)
{
	if(host == server.getServerData("server_name")[0])
	{
		for(size_t j = 0; j < server.getLocations().size();j++)
		{
			//function for longest prefix
			if(server.getLocations()[j].getLocationData("location_name")[0] == url)
			{
				location = server.getLocations()[j];
				std::cout << "======>>>>" << location.getLocationData("autoindex")[0] <<"\n";
				std::cout << "======>>>>" << location.getLocationData("location_name")[0] <<"\n";
			}
		}
	}
}
void Request::matchServer()
{
	for(size_t i =0; i<servers.size();i++)
	{
		// servers[i].printLOcationINfo();
		matchLocation(host, servers[i]);
	}
}

int Request::parseRequest(int fd, int &completeRead,std::vector<Server> initServers)
{
	readReqeust(fd,completeRead);
	servers = initServers;
	if(isDone)
	{
		matchServer();
		return 1;
	}
	return 0;
}