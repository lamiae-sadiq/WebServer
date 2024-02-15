/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   response.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lsadiq <lsadiq@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/26 14:35:49 by lsadiq            #+#    #+#             */
/*   Updated: 2024/02/15 17:48:29 by lsadiq           ###   ########.fr       */
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


#define PORT 8080

#define FILE 1
#define DIRECTORY 0
#define NOT_FOUND -1
#define FORBIDDEN -2

typedef struct location
{
	int 						return_;
	size_t						max_body_size;
	std::string 				root;
	std::string 				index;
	std::string 				auto_index;
	std::string 				upload;
	std::string 				location_name;
	std::vector<std::string> 	method;

}location;

class response
{
	public :
		
		std::ifstream ifile;
		std::ofstream ofile;
		std::string target_url;
		std::string	targetUri;
		std::string line, name;
		std::string content_type;
		std::string extention;
		DIR* dir;
		location loc;
		bool close;
		int fd;
		int status_code ;
		int flag;
		static	std::map<std::string, std::string> _mime;

		response();
		~response();
		static	void    fillMime();
		void	methodGet();
		bool    autoIndexCheck();
		bool    allowedMethods();
		void 	check_extention(std::string file);
		void 	listDirectories();
		void 	handel_error();
		void    sendData();
		int 	checkType(std::string path);
};
