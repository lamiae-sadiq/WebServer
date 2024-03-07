/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lsadiq <lsadiq@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/20 17:00:37 by kel-baam          #+#    #+#             */
/*   Updated: 2024/03/04 16:43:34 by lsadiq           ###   ########.fr       */
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
	public:
		Request();
		~Request(){};
		std::string tmpBuff;
		loc location;
		size_t contentLength;
		int parseHeaders(std::string buff,std::vector<Server> initServers);
		int analyseHeaders(std::string buff);
		void storeRequest(std::string line);
		void storeHostHeader(std::string line);
		void storeRequestLineInfo(std::vector<std::string> vec);
		void storeLocation(Server &server,Location iniLocation);
		int getStatus();
		bool getFirstReadBody();
		loc getLocation();
		std::string getHeader(std::string key);
		std::string getMethod();
		std::string getUrl();
		std::string getVersion();
		size_t getContentLength();
		std::string getContentType();
		std::string getUploadType();
		void setFirstReadOfBody(bool init);
		void setHeader(std::string &key,std::string &value);
		void setMethod(std::string &initVar);
		void setUrl(std::string initVar);
		void setVersion(std::string &initVar);
		void matchServer();
		int matchLocation(std::string host,Server server);
		void checkemptyData(std::string &value,std::vector<std::string> vec,int index);
		void checkVersion(std::string &version);
		void checkMethods(std::string method);
		void checkTransferEncoding(std::string value);
		void checkContentLength(std::string length);
		void chekHeaderError(std::string key);
		void printREquest();
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

