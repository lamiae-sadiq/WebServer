/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lsadiq <lsadiq@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/20 17:00:45 by kel-baam          #+#    #+#             */
/*   Updated: 2024/03/24 19:41:42 by lsadiq           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/Request.hpp"

#include <sys/stat.h>
#include <dirent.h>

std::map<std::string, std::string> Request::mime_;

Request::Request()
{
	location.max_body_size = 0;
	location.redirect_code = 0;
	location.auto_index = "off";
	location.index = "";
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
	_isCgi = false;
	_cgiRuning = false;
	matchLocationDone = false;
	sizeBody = 0;
	
	fileExtention();
};

void    Request::fileExtention()
{
    mime_[ "text/html"] = "html";
    mime_[ "text/css"] = "css";
    mime_[ "text/xml"] =     "xml";
    mime_[ "image/gif"] =   "gif";
    mime_[ "image/jpeg"] = "jpeg";
    mime_[ "image/jpg"] = "jpg";
    mime_[ "application/x-javascript"] = "js";
    mime_[ "text/plain"] = "txt";
    mime_[ "text/x-component"] = "htc";
    mime_[ "text/mathml"] = "mml";
    mime_[ "image/png"] = "png";
    mime_[ "image/x-icon"] = "ico";
    mime_[ "image/x-jng"] = "jng";
    mime_[ "image/vnd.wap.wbmp"] =   "wbmp";
    mime_[ "application/mac-binhex40"] = "hqx";
    mime_[ "application/pdf"] = "pdf";
    mime_[ "application/x-cocoa"] = "cco";
    mime_[ "application/x-java-archive-diff"] = "jardiff";
    mime_[ "application/x-java-jnlp-file"] = "jnlp";
    mime_[ "application/x-makeself"] =   "run";
    mime_[ "application/x-perl"] = "pl";
    mime_[ "application/x-pilot"] = "prc";
    mime_[ "application/x-rar-compressed"] = "rar";
    mime_[ "application/x-redhat-package-manager"] = "rpm";
    mime_[ "application/x-sea"] = "sea";
    mime_[ "application/x-shockwave-flash"] = "swf";
    mime_[ "application/x-stuffit"] = "sit";
    mime_[ "application/x-tcl"] =    "tcl";
    mime_[ "application/x-tcl"] =   "tk";
    mime_[ "application/x-x509-ca-cert"] = "der";
    mime_[ "application/x-x509-ca-cert"] = "pem";
    mime_[ "application/x-x509-ca-cert"] = "crt";
    mime_[ "application/x-xpinstall"] = "xpi";
    mime_[ "application/octet-stream"] = "msm";
    mime_[ "audio/mpeg"] = "mp3";
    mime_["video/mp4"] = "mp4";
    mime_[ "audio/x-realaudio"] = "ra";
    mime_[ "video/mpeg"] = "mpeg";
    mime_[ "video/mpeg"] = "mpg";
    mime_[ "video/quicktime"] = "mov";
    mime_[ "video/x-flv"] = "flv";
    mime_[ "application/zip"] = "zip";
    mime_[ "application/octet-stream"] = "deb";
    mime_[ "application/octet-stream"] = "bin";
    mime_[ "application/octet-stream"] = "exe";
    mime_[ "application/octet-stream"] = "dll";
    mime_[ "application/octet-stream"] = "dmg";
    mime_[ "application/octet-stream"] = "eot";
    mime_[ "application/octet-stream"] = "iso";
    mime_[ "application/octet-stream"] = "img";
    mime_[ "application/octet-stream"] = "msi";
    mime_[ "application/octet-stream"] = "msp";
    mime_[ "video/x-msvideo"] = "avi";
    mime_[ "video/x-ms-wmv"] = "wmv";
    mime_[ "video/x-ms-asf"] = "asx";
    mime_[ "video/x-ms-asf"] =   "asf";
    mime_[ "video/x-mng"] = "mng";
}

Request::~Request(){};

std::string Request::getRealPath()
{
	return realPath;
}
std::string Request::getBody()
{
	return body;
}
size_t Request::getSizeBody()
{
	return 	sizeBody;
}
void Request::setBody(std::string intBody)
{
	body = intBody;
}
void Request::setSizeBody(size_t size)
{
	sizeBody = size;
}
void	Request::setCGIRun()
{
	this->_cgiRuning = true;
}

loc Request::getLocation()const
{
	return this->location;
}
void Request::setStatus(int init)
{
	status = init;
}

void Request::setHeader(std::string &key,std::string &value)
{
	Utils::skipSpaces(value);
	if(key == "host" && value.empty())
		throw HttpBadRequest("Bad request");
	headers[key] = value;
}

int Request::getStatus()const
{
	return status;
}
std::string  Request::getQueryString()const
{
	return query;
}
std::string Request::getCookies()const
{
	return cookies;
}
std::string Request::getUri()const
{
	return uri;
}

std::string Request::getHeader(std::string &key)
{
	return headers[key];
}

std::string Request::getMethod()const
{
	return method;
}

std::string Request::getUrl()const
{
	return url;
}
std::string Request::getUploadType()const
{
	return uplod_type;
}
std::string Request::getVersion()const
{
	return version;
}
bool Request::getFirstReadBody()const
{
	return firstReadOfBody;
}
size_t Request::getContentLength()const
{
	return contentLength;
}
std::string Request::getContentType()const
{
    return content_Type;
}
bool Request::getCgiRuning()
{
	return _cgiRuning;
}
bool Request::getIsCgi()
{
	return _isCgi;
};

void Request::setFirstReadOfBody(bool init)
{
	firstReadOfBody = init;
}

void Request::setUrl(std::string initVar)
{
	size_t index;
	
	std::string allowedCharacter ="ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789-._~:/?#[]@!$&'()*+,;=%";
	if(initVar[0] != '/')
		throw  HttpBadRequest("Bad request");
	if(initVar.length() > 2048)
		throw HttpUriTooLong("Uri Too Long");
	index = initVar.find("?");
	if (index != std::string::npos)
	{
		query = initVar.substr(index + 1);
		url = initVar.substr(0, index);
	}
	else
		url = initVar;
	url = decodingUri(url);
	if(url.find_first_not_of(allowedCharacter)!= std::string::npos)
		throw  HttpBadRequest("Bad request");
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
	method = initVar;
	checkMethods(initVar);
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
	
	if(line.empty())
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
}

std::string Request::decodingUri(std::string str)
{
    size_t index = 0;
	char *parse;
    unsigned char  ch;
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
        tmpStr+= ch;
    }
	return tmpStr;
}

void Request::checkTransferEncoding(std::string value)
{
	Utils::skipSpaces(value);
	if(value != "chunked")
		throw HttpNotImplemented("Not Implemented");
	if(value.empty())
		throw HttpBadRequest("Bad request");
	uplod_type =  "Chunk";
}

void Request::checkContentLength(std::string length)
{
	long long  len;
	
	Utils::skipSpaces(length);
	len = Utils::stringToLongLong(length);
	
	if(!Utils::isInteger(length) || length.empty())
		throw  HttpBadRequest("Bad request");
	if(!Utils::checkOverflowError(length,len))
		throw HttpNotImplemented("Error: length overflowed\n"); 
	if(uplod_type.empty())
	{
		uplod_type ="length";
		contentLength = len;	
	}
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
	
	if(contetType.empty())
		throw HttpBadRequest("Bad request");
	content_Type = contetType;
}

bool Request::getMatchedLocation()
{
	return matchLocationDone;
}

std::string Request::getMimeType(std::string key)
{
	return mime_[key];
}

std::map<std::string, std::string>	Request::getsm()
{
	return mime_;
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
		Utils::strToLower(key);
	}
	else
		Utils::strToLower(key);
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

	if(iniLocation.getLocationData("return").size() == 2)
		location.redirect_code = atoi(iniLocation.getLocationData("return")[0].c_str());
	if(iniLocation.getLocationData("return").size() == 2)
		location.redirect_path = iniLocation.getLocationData("return")[1];
	if(!server.getServerData("client_max_body_size")[0].empty())
		location.max_body_size = Utils::stringToLongLong(server.getServerData("client_max_body_size")[0].c_str());
	if(server.getErrorPage().size() > 0)
		location.error_pages = server.getErrorPage();
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
	if(iniLocation.getLocationData("php").size() > 0)
		location.cgi["php"] = iniLocation.getLocationData("php")[0];
	if(iniLocation.getLocationData("py").size() > 0)
		location.cgi["py"] = iniLocation.getLocationData("py")[0];
}

int LocationLongestPrefix(std::string locationName,std::string Url)
{
	int countMatchingCharacter;
	size_t lenLocation = locationName.length();
	size_t lenUrl = Url.length();
	size_t maxLen = std::max(lenLocation,lenUrl);
	countMatchingCharacter = 0;
	
	if(locationName == "/")
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




void Request::getREalPath()
{
	std::string targetUri;
	char real_Pathh[PATH_MAX];
	char *real_Path;
	char *currentDir;
	
	targetUri = location.root + url.substr(location.location_name.size());
	realPath = targetUri;
	real_Path = realpath(targetUri.c_str(),real_Pathh);
	currentDir = realpath(".",NULL);
	
	if(real_Path)
	{
		realPath = real_Pathh;
		if(targetUri[targetUri.length() - 1] == '/')
			realPath += '/';
		else
			realPath = real_Pathh;
		if(realPath.find(currentDir) != 0)
			throw HttpForbidden("Forbidden\n");
	}
}


void Request::matchLocation(Server currentServer)
{
	std::string locationName;
	std::string tmpUrl;
	size_t indexSlach;
	bool isMatched;
	
	isMatched = false;
	tmpUrl = url;
	while(!isMatched)
	{
		for(size_t j = 0; j < currentServer.getLocations().size();j++)
		{
			locationName = currentServer.getLocations()[j].getLocationData("location_name")[0];
			if(locationName == tmpUrl)
			{
				storeLocation(currentServer,currentServer.getLocations()[j]);
				isMatched = true;
				break;
			}
		}
		if(tmpUrl[tmpUrl.size() - 1] == '/')
			tmpUrl = tmpUrl.substr(0,tmpUrl.size() - 1);
		else
		{
			indexSlach =  tmpUrl.find_last_of("/");
			if(indexSlach != std::string::npos)
				tmpUrl = tmpUrl.substr(0,indexSlach + 1);
		}	
	}
	getREalPath();
}

Server Request::matchServer()
{
	for(size_t i =0; i<servers.size();i++)
	{
		if(servers[i].getServerData("server_name").size() > 0 && host == servers[i].getServerData("server_name")[0])
			return servers[i];
	}
	return servers[0];
}

void Request::checkStoreData()
{
	if(headers.find("host") ==  headers.end())
		throw HttpBadRequest("Bad request");
	if(headers.find("host")!= headers.end())
		storeHostHeader(headers["host"]);
	if(headers.find("content-length")!= headers.end())
		checkContentLength(headers["content-length"]);
	if(headers.find("content-type")!= headers.end())
		checkContentType(headers["content-type"]);
	if(headers.find("transfer-encoding") != headers.end())
		checkTransferEncoding(headers["transfer-encoding"]);
	if(headers.find("cookie")!= headers.end())
	{
		Utils::skipSpaces(headers["cookie"]);
		cookies = headers["cookie"];
	}
	if(method == "POST" && uplod_type.empty())
		throw HttpLengthRequired("length Required");
	if(method == "POST" && (uplod_type == "length") && !contentLength)
		throw HttpBadRequest("Bad request");
}

int Request::parseHeaders(std::string buff,std::vector<Server> initServers)
{
	try
	{
		if(!matchLocationDone)
		{
			servers = initServers;
			if(analyseHeaders(buff))
			{
				checkStoreData();
				matchedServer = matchServer();
				matchLocation(matchedServer);
				matchLocationDone = true;
				status = 1;
			}
			return 1;
		}
	}
	catch(HttpException& e)
	{
		status = 1;
		return e.getStatusCode();
	}
	return 1;
}