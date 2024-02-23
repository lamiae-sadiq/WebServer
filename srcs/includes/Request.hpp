/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kel-baam <kel-baam@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/20 17:00:37 by kel-baam          #+#    #+#             */
/*   Updated: 2024/02/23 14:45:49 by kel-baam         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./parse.hpp"
#include <sys/socket.h>
#include <unistd.h>
#include <string.h>
#include <map>

class Request
{
	private:
		std::map<std::string,std::string> headers;
		std::string method;
		std::string url;
		std::string version;
		std::string contentLength;
		std::string contentType;
		std::string host;
		std::vector<Server> servers;
		int isChunke;
		int isDone;
		int countHeaders;
	public:
		Location location;
		// Request(std::vector<Server> initServers)
		// {
			
		// 	servers = initServers;
		// }
		std::string tmpBuff;
		Request()
		{
			tmpBuff = "";
			countHeaders = 0;
			isDone = 0;
			isChunke = 1;
		};
		
		void setHeader(std::string &key,std::string &value);
		std::string getHeader(std::string key);
		std::string getMethod();
		std::string getUrl();
		std::string getVersion();
		std::string getContentLength();
		std::string getContentType();
		void setMethod(std::string initVar);
		void setUrl(std::string initVar);
		void setVersion(std::string initVar);
		void setContentLength(std::string initVar);
		void setContentType(std::string initVar);
		int parseRequest(int fd, int &completeRead,std::vector<Server> servers);
		void readReqeust(int &fd,int &completeRead);
		void analyseRequest(char * buff, int &completeRead);
		void storeRequest(std::string line);
		void storeHostHeader(std::string line);
		void matchServer();
		void storeRequestLineInfo(std::vector<std::string> vec);
		void matchLocation(std::string host,Server server);
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

