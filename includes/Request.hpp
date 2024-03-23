/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lsadiq <lsadiq@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/20 17:00:37 by kel-baam          #+#    #+#             */
/*   Updated: 2024/03/23 21:41:17 by lsadiq           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */



#pragma once


#include "./Parse.hpp"
#include <sys/socket.h>
#include <unistd.h>
#include <string.h>
#include <map>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "HttpException.hpp"
#include <iostream>
#include <sstream>
#include <limits.h>
#define FILE 1
#define FILE_PER 2
#define DIRECTORY 0
#define DIR_PER 3


typedef struct loc
{
	long long int				max_body_size;
	std::string 				root;
	std::string 				index;
	std::string 				auto_index;
	bool		 				allowedUpload;
	std::string 				upload;
	std::string 				location_name;
	std::vector<std::string> 	method;
	int							redirect_code;
	std::string 				redirect_path;
	std::map<std::string,std::string> cgi;
	std::map<int,std::string> error_pages;
	
}loc;

class Request
{
	private:
		std::vector<Server> servers;
		Server matchedServer;
		std::string host;
		std::string method;
		std::string url;
		std::string version;
		std::string content_Type;
		std::string uplod_type;
		int status;
		int countHeaders;
		bool firstReadOfBody;
		std::string query;
		std::string cookies;
		std::string uri;
		bool _isCgi;
		bool _cgiRuning;
		std::string realPath;
		static	std::map<std::string, std::string> mime_;
		std::string body;
		size_t sizeBody;
		bool matchLocationDone;
		
	public:
		std::map<std::string,std::string> headers;
		struct timeval start_time, end_time;
		Request();
		~Request();
		std::string tmpBuff;
		loc location;
		std::string getRealPath();
		std::string getBody();
		size_t getSizeBody();
		void setBody(std::string intBody);
		void setSizeBody(size_t size);
		long long int contentLength;
		int parseHeaders(std::string buff,std::vector<Server> initServers);
		int analyseHeaders(std::string buff);
		void storeRequest(std::string line);
		void storeHostHeader(std::string line);
		void storeRequestLineInfo(std::vector<std::string> vec);
		void storeLocation(Server &server,Location iniLocation);
		int getStatus()const;
		bool getMatchedLocation();
		void setStatus(int init);
		bool getFirstReadBody()const;
		loc getLocation()const;
		void	setCGIRun();
		bool getCgiRuning();
		bool getIsCgi();
		std::string getHeader(std::string &key);
		std::string getMethod()const;
		std::string getUrl()const;
		std::string getVersion()const;
		size_t getContentLength()const;
		std::string getContentType()const;
		std::string getUploadType()const;
		void setFirstReadOfBody(bool init);
		void setHeader(std::string &key,std::string &value);
		void setMethod(std::string &initVar);
		void setUrl(std::string initVar);
		void setVersion(std::string &initVar);
		Server matchServer();
		void matchLocation(Server currentServer);
		void checkVersion(std::string &version);
		void checkMethods(std::string method);
		void checkTransferEncoding(std::string value);
		void checkContentLength(std::string length);
		void chekHeaderError(std::string key);
		void printREquest();
		void checkContentType(std::string &contetType);
		std::string decodingUri(std::string str);	
		void checkStoreData();	
		std::string getCookies()const;
		std::string getUri()const;
		std::string getQueryString()const;
		void getREalPath();
		std::string getMimeType(std::string key);
		std::map<std::string, std::string>	getsm();
		static	void    fileExtention();
};

