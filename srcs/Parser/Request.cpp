/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lsadiq <lsadiq@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/20 17:00:45 by kel-baam          #+#    #+#             */
/*   Updated: 2024/03/11 16:10:38 by lsadiq           ###   ########.fr       */
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
	contentLength = 0;
	tmpBuff = "";
	host = "";
	method = "";
	countHeaders = 0;
	status = 0;
	uplod_type ="";
	content_Type ="";
	version ="";
	url ="";
	query ="";
	firstReadOfBody = true;
};

loc Request::getLocation()
{
	return this->location;
}

void Request::setHeader(std::string &key,std::string &value)
{
	Utils::skipSpaces(value);
	if(key == "host" && value.empty())
		throw HttpBadRequest("Bad request");
	headers[key] = value;
}

int Request::getStatus()
{
	return status;
}

std::string Request::getUri()
{
	return uri;
}

std::string Request::getCookies()
{
	return cookies;
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
std::string Request::getContentType(){
    return content_Type;
}
std::string Request::getQueryString()
{
	return query;
}


void Request::setFirstReadOfBody(bool init)
{
	firstReadOfBody = init;
}

void Request::setUrl(std::string initVar)
{
	size_t index;
	
	std::string allowedCharacter ="ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789 ._~:/?#[]@!$&'()*+,;=%";
	if(initVar[0] != '/')
		throw  HttpBadRequest("Bad request");
	if(initVar.length() > 2048)
		throw HttpUriTooLong("Uri Too Long");
	url = decodingUri(initVar);
	index = url.find("?");
	if(url.find_first_not_of(allowedCharacter)!= std::string::npos)
		throw  HttpBadRequest("Bad request");
	if (index != std::string::npos)
	{
		query = url.substr(index + 1);
		url = url.substr(0, index);
	}
}

void Request::checkVersion(std::string &version)
{
	std::string versionNum;
	size_t index ;

	index = version.find("HTTP/");
	if(index != std::string::npos)
		versionNum = version.substr(index+5);
	if(index != 0 || !Utils::isInteger(versionNum) || version.length() <= 5 || std::count(versionNum.begin(),versionNum.end(),'.') > 1)
		throw HttpBadRequest("Bad request");	
	if(versionNum != "1.1")
		throw  HttpVersionNotSupported("Http Version Not Supported");
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
		throw  HttpBadRequest("Bad request");
	if(method != methods[0] && method != methods[1] && method != methods[2])
		throw  HttpNotImplemented("Not Implemented");
}

void Request::setMethod(std::string &initVar)
{
	checkMethods(initVar);
	method = initVar;
}

void Request::storeRequestLineInfo(std::vector<std::string> vec)
{
	if(vec.size() != 3)
		throw  HttpBadRequest("Bad request");
	setMethod(vec[0]);
	setUrl(vec[1]);
	setVersion(vec[2]);
}
		
void Request::storeHostHeader(std::string line)
{
	size_t index;
	if(!host.empty() || line.empty())
		throw  HttpBadRequest("Bad request");
	Utils::skipSpaces(line);
	index = line.find(":");
	std::string port;
	if(index != std::string::npos)
	{
		host = line.substr(0,index);
		port = line.substr(index+1);
	}
	else
		host = line;
	std::cout << "||" << host <<"||\n";
}

std::string Request::decodingUri(std::string str)
{
    size_t index = 0;
	char *parse;
    char ch;
    std::string tmpStr;
    std::stringstream ss(str);
    while(index < str.length())
    {
        ss >> ch;
        if(ch == '%')
        { 
			if(Utils::isHex(str[index + 1]) == true && Utils::isHex(str[index +2]) == true)
			{
				ch =  std::strtol(str.substr(index + 1,2).c_str(),&parse,16);
				index+=2;
				ss.ignore(2);
			}
        }
            index++;
        tmpStr.push_back(ch);
    }
	return tmpStr;
}

void Request::checkTransferEncoding(std::string value)
{
	Utils::skipSpaces(value);
	if(value != "chunked")
		throw HttpNotImplemented("Not Implemented"); //to be checked
	if(uplod_type == "length" || value.empty())
		throw HttpBadRequest("Bad request");
	uplod_type =  "Chunk";
}

void Request::checkContentLength(std::string length)
{

	Utils::skipSpaces(length);
	if(!Utils::isInteger(length) || length.empty())
		throw  HttpBadRequest("Bad request");
	contentLength = atol(length.c_str());
	uplod_type ="length";
}


void	Request::chekHeaderError(std::string key)
{
	if(key.find_first_of(" \t")!= std::string::npos)
		throw HttpBadRequest("Bad request");
	if(key == "host" && headers.count("host") == 1)
		throw HttpBadRequest("Bad request");
	if(key == "content-length" && headers.count("content-length") == 1)
		throw HttpBadRequest("Bad request");
	if(key == "transfer-encoding" &&  headers.count("transfer-encoding") == 1)
		throw HttpBadRequest("Bad request");
}
void Request::checkContentType(std::string &contetType)
{
	Utils::skipSpaces(contetType);
	if(contetType.find("boundary=") != std::string::npos)
		throw  HttpNotImplemented("Not Implemented");
	if(contetType.empty())
		throw HttpBadRequest("Bad request");
		
	content_Type = contetType;
}

void Request::storeRequest(std::string line)
{
	size_t index = line.find(":");
	std::string key = line;
	if(countHeaders)
		Utils::strToLower(line);
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
	setHeader(key,line);
}

int Request::analyseHeaders(std::string buff)
{
	size_t index = 1;
	std::string requests;
	requests = tmpBuff + buff;

 	while(index != std::string::npos)
	{
		index = requests.find("\r\n");
		if(index != std::string::npos)
		{	
			if(!index)
			{
				tmpBuff = std::string(requests, index+2);
				return 1;
			}
			else
			{
				storeRequest(requests.substr(0,index));
				requests = requests.substr(index + 2);
			}
		}
		else if(!requests.empty())
		{
			tmpBuff = requests;
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
	if(iniLocation.getLocationData("cgi").size() > 0)
		location.cgi[iniLocation.getLocationData("cgi")[0]] = iniLocation.getLocationData("cgi")[1];
		
}

void  Request::printREquest()
{
	std::cout << "request info======\n";
	std::cout <<"version======>|" <<version << "|\n";
	std::cout << "url=======>|" << url << "|\n";
	std::cout << "content_Type======>|" << content_Type << "|\n";
	std::cout << "uplod_type======>|" << uplod_type << "|\n";
	std::cout << "content length======>|" << contentLength << "|\n";
	std::cout << "max_body_size ======>>>>|" <<  location.max_body_size <<"|\n";
	std::cout << "root ======>>>>|" <<  location.root<<"|\n";
	std::cout << "index ======>>>>|" <<  location.index<<"|\n";
	std::cout << "auto_index ======>>>>|" <<  location.auto_index<<"|\n";
	std::cout << "upload ======>>>>|" <<  location.upload<<"|\n";
	std::cout << "location_name ======>>>>|" <<  location.location_name<<"|\n";
	std::cout << "allowedUpload ======>>>>|" <<  location.allowedUpload<<"|\n";
	std::cout << "redirect_path ======>>>>|" << location.redirect_path<<"|\n";
	std::cout << "redirect_code ======>>>>|" << location.redirect_code<<"|\n";
	
	for(size_t i = 0;i < location.method.size();i++)
	{
		std::cout << "methods ======>>>>|" <<  location.method[i] << "|\n";
	}
	std::map<std::string, std::string>::iterator it = location.cgi.begin();
	while(it != location.cgi.end())
	{
		std::cout << (it->first) <<" " << (it->second) << "\n";
		it++;
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

void Request::matchLocation(Server currentServer)
{
	size_t maxMatcher= 0;
	size_t countMatcher;
	std::string locationName;

	for(size_t j = 0; j < currentServer.getLocations().size();j++)
	{
		locationName = currentServer.getLocations()[j].getLocationData("location_name")[0];
		countMatcher = LocationLongestPrefix(locationName, url);
		if(countMatcher > 0)
		{
			if(countMatcher > maxMatcher)
			{
				storeLocation(currentServer,currentServer.getLocations()[j]);
				maxMatcher = countMatcher;
			}
				break;
		}
	}
}

Server Request::matchServer()
{

	for(size_t i =0; i<servers.size();i++)
	{
		std::cout << "host|" << host << "|"<<"|" << servers[i].getServerData("server_name")[0] <<"|\n";
		if(host == servers[i].getServerData("server_name")[0])
			return servers[i];
	}
	return servers[0];
}

void Request::checkStoreData()
{
	if(headers.find("host")!= headers.end())
		storeHostHeader(headers["host"]);
	if(headers.find("content-length")!= headers.end())
		checkContentLength(headers["content-length"]);
	if(headers.find("content-type")!= headers.end())
		checkContentType(headers["content-type"]);
	if(headers.find("transfer-encoding") != headers.end())
		checkTransferEncoding(headers["transfer-encoding"]);
	if(uplod_type.empty() && method == "POST")
		throw HttpBadRequest("Bad request");
	if(headers.find("cookie")!= headers.end())
	{
		Utils::skipSpaces(headers["cookie"]);
		cookies = headers["cookie"];
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
				checkStoreData();
				Server server = matchServer();
				matchLocation(server);
				// printREquest();
				status = 1;
				return 1;
			}
		}
	}
	catch(HttpException& e)
	{
		status = 1;
		return e.getStatusCode();
	}
	return 1;
}