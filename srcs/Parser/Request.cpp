/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lsadiq <lsadiq@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/20 17:00:45 by kel-baam          #+#    #+#             */
/*   Updated: 2024/03/01 13:22:56 by lsadiq           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/Request.hpp"



Request::Request()
{
	location.max_body_size = 0;
	location.redirect_code = 0;
	location.auto_index = "off";
	location.index = "";
	location.allowedUpload = false;
	location.upload = "";
	location.location_name = "";
	location.redirect_path = "";
	tmpBuff = "";
	host = "";
	method = "";
	countHeaders = 0;
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
std::string Request::getUploadType()
{
	return uplod_type;
}
std::string Request::getVersion()
{
	return version;
}
bool Request::getFirstReadBody()
{
	return firstReadOfBody;
}
size_t Request::getContentLength()
{
	return contentLength;
}


void Request::setFirstReadOfBody(bool init)
{
	firstReadOfBody = init;
}
void Request::setUrl(std::string initVar)
{
	if(initVar[0] != '/')
		throw 400;
	url = initVar;
}


void Request::checkVersion(std::string &version)
{
	std::string versionNum;
	size_t index ;

	index = version.find("HTTP/");
	if(index != std::string::npos)
		versionNum = version.substr(index+5);
	if(index != 0 || !Utils::isInteger(versionNum) || version.length() <= 5 || std::count(versionNum.begin(),versionNum.end(),'.') > 1)
		throw 400;	
	if(versionNum != "1.1")
		throw 505;
}

void Request::setVersion(std::string &initVar)
{
	checkVersion(initVar);
	version = initVar;
}

void Request::checkMethods(std::string method)
{
	const char *methods[] ={"GET","POST","DELETE","PUT","PATCH","HEAD","OPTIONS","TRACE","CONNECT"};
	std::vector<std::string> copyMethods(methods, methods + sizeof(methods) / sizeof(methods[0]));

	if(!std::count(copyMethods.begin(), copyMethods.end(),method))
		throw 400; //400 bad request
	if(method != methods[0] && method != methods[1] && method != methods[2])
		throw 501;

}
void Request::setMethod(std::string &initVar)
{
	checkMethods(initVar);
	method = initVar;
}
void Request::storeRequestLineInfo(std::vector<std::string> vec)
{
	if(vec.size() != 3)
		throw 400; //400
	setMethod(vec[0]);
	setUrl(vec[1]);
	setVersion(vec[2]);
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

void Request::checkTransferEncoding(std::string value)
{
	Utils::skipSpaces(value);
	if(value != "chunked")
		throw 501; //to be checked
	uplod_type =  "Chunk";
}
void Request::checkContentLength(std::string length)
{
	Utils::skipSpaces(length);
	if(!Utils::isInteger(length))
		throw 400;
	contentLength = atol(length.c_str());
	uplod_type ="length";
}

void	Request::chekHeaderError(std::string key)
{
	if(key.find_first_of(" \t")!= std::string::npos)
		throw 400;
}
 	
void Request::storeRequest(std::string line)
{
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
	chekHeaderError(key);
	if(key == "Host")
		storeHostHeader(line);
	else if(key == "Content-Length")
		checkContentLength(line);
	if(key == "Content-Type") // what should i chek here
		content_Type = line;
	else if(key == "Transfer-Encoding") // i should check if is transfer encoding but not chunked
		checkTransferEncoding(line);
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


void Request::storeLocation(Server &server, Location iniLocation)
{
	
	if(iniLocation.getLocationData("return").size() == 1)
		location.redirect_code = atoi(iniLocation.getLocationData("return")[0].c_str());
	if(iniLocation.getLocationData("return").size() == 2)
		location.redirect_path = iniLocation.getLocationData("return")[1];
	if(!server.getServerData("client_max_body_size")[0].empty())
		location.max_body_size = atol(server.getServerData("client_max_body_size")[0].c_str());
	if(iniLocation.getLocationData("root").size() == 1)
		location.root =	iniLocation.getLocationData("root")[0];
	if(iniLocation.getLocationData("index").size() == 1)
		location.index =  iniLocation.getLocationData("index")[0];
	if(iniLocation.getLocationData("autoindex").size() == 1)
		location.auto_index =  iniLocation.getLocationData("autoindex")[0];
	if(iniLocation.getLocationData("upload").size() == 1)
		location.upload = iniLocation.getLocationData("upload")[0];
	if(iniLocation.getLocationData("location_name").size() == 1)
		location.location_name = iniLocation.getLocationData("location_name")[0];
	if(iniLocation.getLocationData("http_methods").size() > 0)
		location.method =  iniLocation.getLocationData("http_methods");
	if(!(iniLocation.getLocationData("allowedUpload").empty()) &&  iniLocation.getLocationData("allowedUpload")[0] == "on")
		location.allowedUpload = true;
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
	try
	{
		if(!status)
		{
			servers = initServers;
			if(analyseHeaders(buff))
			{
				matchServer();
				if(host.empty())
					throw 404;
				printREquest();
				status = 1;
				return 1;
			}
		}
	}
	catch(int statusCode)
	{
		status = 1;
		return statusCode;
	}
	return 1;
}