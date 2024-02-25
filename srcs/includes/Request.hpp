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

#include "./parse.hpp"
#include <sys/socket.h>
#include <unistd.h>
#include <string.h>
#include <map>

typedef struct loc
{
	int 						return_;
	int						max_body_size;
	std::string 				root;
	std::string 				index;
	std::string 				auto_index;
	bool		 				allowedUpload; //string
	std::string 				upload;
	std::string 				location_name;
	std::vector<std::string> 	method;
	
}loc;

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
		struct loc location;
		int isChunke;
		int isDone;
		int status;
		int countHeaders;
	public:
		std::string tmpBuff;
		int getStatus()
		{
			return status;
		}
		Request()
		{
			tmpBuff = "";
			countHeaders = 0;
			isDone = 0;
			isChunke = 1;
			status = 0;
		};
		loc get()
		{
			return this->location;
		}
		void setHeader(std::string &key,std::string &value);
		std::string getHeader(std::string key);
		std::string getMethod();
		std::string getUrl();
		std::string getVersion();
		std::string getContentLength();
		std::string getContentType();
		std::string checkemptyData(std::vector<std::string> vec);
		void setMethod(std::string initVar);
		void setUrl(std::string initVar);
		void setVersion(std::string initVar);
		void setContentLength(std::string initVar);
		void setContentType(std::string initVar);
		int parseHeaders(std::string buff,std::vector<Server> servers);
		void analyseHeaders(std::string buff);
		void storeRequest(std::string line);
		void storeHostHeader(std::string line);
		void matchServer();
		void storeRequestLineInfo(std::vector<std::string> vec);
		int matchLocation(std::string host,Server server);
		void storeLocation(Server &server,Location iniLocation);
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

