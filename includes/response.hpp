/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   response.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lsadiq <lsadiq@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/26 14:35:49 by lsadiq            #+#    #+#             */
/*   Updated: 2024/02/23 18:05:09 by lsadiq           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <iostream>
#include <string>
#include <fstream>
#include <stdexcept>
#include <sys/socket.h>
#include <stdlib.h>
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <unistd.h>
#include <netinet/in.h>
#include <vector>
#include <string.h>
#include <iostream> 
#include <dirent.h>
#include <signal.h>
#include <sstream>
#include <map>
#include "Request.hpp"

#define PORT 8080

#define FILE 1
#define DIRECTORY 0
#define NOT_FOUND -1
#define FORBIDDEN -2

// typedef struct location
// {
// 	int 						return_;
// 	size_t						max_body_size;
// 	std::string 				root;
// 	std::string 				index;
// 	std::string 				auto_index;
// 	bool		 				allowedUpload;
// 	std::string 				upload;
// 	std::string 				location_name;
// 	std::vector<std::string> 	method;
// }location;

class response
{
	private :
		size_t 			body_length;
		std::string 	target_url;
		std::string 	content_type;
		std::string		uplod_type;

		// location 		loc;
		Request         request;

		std::ofstream 	upfile;
		std::string 	chunkSizeStr;
		bool 			close;
		DIR* 			dir;
		int 			flag;
		int 			status_code ;
		int 			fd;
		std::ifstream 	ifile;
		std::ofstream 	ofile;
		std::string		targetUri;
		std::string 	line, name;
		std::string 	extention;
		std::string		pathUpload;
		static			std::map<std::string, std::string> _mime;
		std::string		uploadFileNmae;
		std::string		fileType;
		std::string		body;
		size_t chunkSize;
		int ihex;
	public :

		response();
		response(Request initRequest)
		{
			request = initRequest;
		}
		~response();
		void	methodGet();
		void	methodPost();
		void	setFd(int fd){
			this->fd = fd;
		};
		Request getRequest()
		{
			std::cout << "req\n";
			return request;
		}
		bool    autoIndexCheck();
		bool    allowedMethods();
		bool    checkUpload();
		void 	check_extention(std::string file);
		void 	listDirectories();
		void 	handel_error();
		void    sendData();
		int 	checkType(std::string path);
		static	void    fillMime();

		//geters //
		std::string getTargetUrl();
		std::string getTargetUri();
		std::string getLine();
		std::string getName();
		std::string getContentType();
		std::string getExtension();
		std::string	getUploadFN();
		void setRequest(Request initReq);
		int getFlag();
		int getStatusCode();
		int getFd();
		bool getClose() ;
		std::ifstream& getIfile();
		std::ofstream& getOfile();
		DIR* getDir();
		loc getLocation();
		std::string newUpload();
		std::map<std::string, std::string> getMime();
		void createFile();
		std::string	generateName();
		void parsLength(char *con, size_t& index, size_t size);
		void parseChunk(char *con, size_t& index, size_t size);
		void	Delete();
		void init();
};
