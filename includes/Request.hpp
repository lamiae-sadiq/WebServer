/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kel-baam <kel-baam@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/20 17:00:37 by kel-baam          #+#    #+#             */
/*   Updated: 2024/02/24 17:51:01 by kel-baam         ###   ########.fr       */
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
		int isDone;
		int status;
		int countHeaders;
		bool firstReadOfBody;
	public:
		std::string tmpBuff;
		loc location;
		size_t contentLength;
		int getStatus();
		Request();
		// std::string& getTmpBuff()
		// {
		// 	return tmpBuff;
		// }
		bool getFirstReadBody()
		{
			return firstReadOfBody;
		}
		void setFirstReadOfBody(bool init)
		{
			firstReadOfBody = init;
		}
		loc getLocation();
		void setHeader(std::string &key,std::string &value);
		std::string getHeader(std::string key);
		std::string getMethod();
		std::string getUrl();
		std::string getVersion();
		size_t getContentLength();
		std::string getContentType();
		std::string checkemptyData(std::vector<std::string> vec,int index);
		void setMethod(std::string initVar);
		void setUrl(std::string initVar);
		void setVersion(std::string initVar);
		int parseHeaders(std::string buff,std::vector<Server> initServers);
		int analyseHeaders(std::string buff);
		void storeRequest(std::string line);
		void storeHostHeader(std::string line);
		void matchServer();
		void storeRequestLineInfo(std::vector<std::string> vec);
		int matchLocation(std::string host,Server server);
		void storeLocation(Server &server,Location iniLocation);
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

