/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   webs.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lsadiq <lsadiq@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/26 14:35:49 by lsadiq            #+#    #+#             */
/*   Updated: 2024/02/12 21:25:34 by lsadiq           ###   ########.fr       */
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
#include <signal.h>
#include <sstream>
#include <map>


#define PORT 8080

#define FILE 1
#define DIRECTORY 0
#define NOT_FOUND -1

class location
{
	public :
		std::ifstream ifile;
		std::ofstream ofile;
		std::string index;
		std::string	auto_index;
		std::vector<std::string>	method;
		std::string root;
		std::string location_name;
		std::string target_url;
		std::string	newlocation;
		std::string uploade;
		std::string line, name;
		std::string return_;
		std::string path;
		std::fstream file;
		std::string content_type;
		std::string extention;
		bool close;
		int fd;
		int status_code ;
		int flag;
	static	std::map<std::string, std::string> _mime;
	static	void    fillMime();
		location();
		~location();
		struct data{int y;};

		void	methodGet();
		bool    autoIndexCheck();
		bool    allowedMethods();
		void 	check_extention(std::string file);
		void 	listDirectories();
		void 	handel_error();
		void    sendData();
};
