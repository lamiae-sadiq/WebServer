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
	status = 0;
	uplod_type ="";
	content_Type ="";
	version ="";
	url ="";
	firstReadOfBody = true;
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
	{
		uplod_type ="length";
		contentLength = atoi(line.c_str());
	}
	if(key == "Content-Type")
	{
		content_Type = line;
	}
	else if(key == "Transfer-Encoding" && line == "chunked") // i should check if is transfer encoding but not chunked
		uplod_type = "Chunk";
	// else if(key == )
	//check headers key : value
	//setHeader(key,line);
}




int Request::analyseHeaders(std::string buff)
{
	size_t index = 1;

	std::string requests,str;
	requests = tmpBuff + buff;
	size_t startBody = requests.find("\r\n\r\n");
	str = requests;
	if(startBody != std::string::npos)
		str = std::string(requests,0,startBody + 4);
	while(index != std::string::npos)
	{
		index = str.find("\r\n");
		if(index != std::string::npos)
		{
			if(!str.find("\r\n") && startBody!= std::string::npos)
			{
				tmpBuff = std::string(buff, startBody + 4);
				return 1;
			}
			else
			{
				storeRequest(str.substr(0,index));
				str = str.substr(index + 2);
			}
		}
		else if(!str.empty())
		{
			tmpBuff = str;
		}
		else
			tmpBuff = "";
	}
	return 0;
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
	location.root =	checkemptyData(iniLocation.getLocationData("root"),0);
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
	std::cout << "request info======\n";
	std::cout <<"version======>" <<version << "\n";
	std::cout << "url=======>" << url << "\n";
	std::cout << "content_Type======>" << content_Type << "\n";
	std::cout << "uplod_type======>" << uplod_type << "\n";
	std::cout << "content length======>" << contentLength << "\n";
	std::cout << "max_body_size ======>>>>" <<  location.max_body_size <<"\n";
	std::cout << "root ======>>>>" <<  location.root<<"\n";
	std::cout << "index ======>>>>" <<  location.index<<"\n";
	std::cout << "auto_index ======>>>>" <<  location.auto_index<<"\n";
	std::cout << "upload ======>>>>" <<  location.upload<<"\n";
	std::cout << "location_name ======>>>>" <<  location.location_name<<"\n";
	std::cout << "allowedUpload ======>>>>" <<  location.allowedUpload<<"\n";
	std::cout << "redirect_path ======>>>>" << location.redirect_path<<"\n";
	std::cout << "redirect_code ======>>>>" << location.redirect_code<<"\n";

	for(size_t i = 0;i < location.method.size();i++)
	{
		std::cout << "methods ======>>>>" <<  location.method[i] << "\n";
	}
}

int LocationLongestPrefix(std::string locationName,std::string Url)
{
	int countMatchingCharacter = 0;
	size_t lenLocation = locationName.length();
	size_t lenUrl = Url.length();
	size_t maxLen = std::max(lenLocation,lenUrl);

	if(locationName =="/")
		return 1;
	for(size_t i = 0;i < maxLen;i++)
	{
		if(locationName[i] != Url[i])
		{
			if( Url[i] =='/')
				return countMatchingCharacter;
			return 0;
		}
		countMatchingCharacter++;
	}
	return countMatchingCharacter;
}
int Request::matchLocation(std::string host,Server server)
{
	size_t maxMatcher= 0;
	size_t countMatcher;
	std::string locationName;

	if(host == server.getServerData("server_name")[0])
	{
		for(size_t j = 0; j < server.getLocations().size();j++)
		{
			locationName = server.getLocations()[j].getLocationData("location_name")[0];
			countMatcher = LocationLongestPrefix(locationName, url);
			if(countMatcher > 0)
			{
				if(countMatcher > maxMatcher)
				{
					storeLocation(server,server.getLocations()[j]);
					maxMatcher = countMatcher;
				}
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
	if(!status)
	{
		servers = initServers;
		if(analyseHeaders(buff))
		{
			matchServer();
			status = 1;
			return 1;
		}
	}
	return 1;
}