/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kel-baam <kel-baam@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/20 17:00:37 by kel-baam          #+#    #+#             */
/*   Updated: 2024/03/13 12:13:39 by kel-baam         ###   ########.fr       */
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

typedef struct loc
{
	unsigned long long							max_body_size;
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
	
}loc;

class Request
{
	private:
		std::map<std::string,std::string> headers;
		std::vector<Server> servers;
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
		
	public:
		struct timeval start_time, end_time;
		Request();
		~Request(){};
		std::string tmpBuff;
		loc location;
		long long int contentLength;
		int parseHeaders(std::string buff,std::vector<Server> initServers);
		int analyseHeaders(std::string buff);
		void storeRequest(std::string line);
		void storeHostHeader(std::string line);
		void storeRequestLineInfo(std::vector<std::string> vec);
		void storeLocation(Server &server,Location iniLocation);
		int getStatus()const;
		void setStatus(int init)
		{
			status = init;
		}
		bool getFirstReadBody()const;
		loc getLocation()const;
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
		class readError:public std::exception 
        {
            const char* what() const throw()
            {
                return "error in reading\n";
            }
        };
		class headerError:public std::exception 
        {
            const char* what() const throw()
            {
                return "error in request header\n";
            }
        };
};

