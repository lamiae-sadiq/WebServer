/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   response.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lsadiq <lsadiq@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/26 14:35:49 by lsadiq            #+#    #+#             */
/*   Updated: 2024/03/18 23:48:24 by lsadiq           ###   ########.fr       */
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
#include "Server.hpp"
#include "cgi.hpp"
#include <ctime>

#define FILE 1
#define DIRECTORY 0
#define NOT_FOUND -1
#define FORBIDDEN -2
// static int i = 0;
#define CGI 0


// class server;
// class Cgi;

class response
{
	private :
		size_t 			body_length;
		std::string 	target_url;
		std::string 	content_type;
		std::string		uplod_type;
		Request         &request;
		std::ofstream 	upfile;
		std::string 	chunkSizeStr;
		// bool 			close;
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
		static			std::map<std::string, std::string> mime_;
		std::string		uploadFileNmae;
		std::string		fileType;
		std::string		body;
		size_t 			chunkSize;
		bool			lastChunk;
		std::ifstream 	infile;
		int 			ihex;
		///////////////////////////////////////
		//cgi
		char** 									env;
		std::map<std::string, std::string>		cgiHeader;
		std::map<std::string, std::string>		_cgiMap;
		int										_fd;
		int 									pid;
		std::string path;
		int 									cgiOutfile;
		std::map<std::string, std::string>		_cgiHeader;
		bool									_cgiStarted;
		bool									_cgiEnded;
		long long								cgiStartTime;
		// long long 								cgiStartTime_1;
		std::string								uplfile;
		bool									_isCgi;
		bool									_isStatus;
		std::ofstream							_cgiFile;
		/*=========================*/
		bool									postDone;
	public :
		std::ifstream cinfile;
		Server serv;
		response(Request& initRequest);
		~response();
		response();
		
		void	methodGet();
		void	methodPost(const char *con, size_t size);
		void	setFd(int fd)
		{
			this->fd = fd;
		};
		void setStatusCode(int initCode)
		{
			status_code = initCode;
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
		static	void    fileExtention();
		std::map<std::string, std::string> getMime_();

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
		void parsLength(const char *con, size_t& index, size_t size);
		void parseChunk(const char *con, size_t& index, size_t size);
		void	Delete();
		void	deleteDir(std::string uri);
		void init();
		void executeMethodes(const char *buff,size_t size,int fd);
		const std::string HTMLPage();
		const std::string    setStatus(int status);
		void ErrorHeader();
		std::string getErrorPage();
		void    sendErrorPage();
		void    allow();
		void	setHeader();
		/////////////////cgi///////////////
		void		setEnv();
        void		executePHP();
        void		executePython();
        char**		getEnv();
		void		cgiSendResponse();
		void		setCgiHeader();
		void		parsecgiFile();
		void		handelCGI();
		bool		_cgiProcess();
		void    	Post();
		template <typename T>
		std::string to_string(T value) {
			std::ostringstream os;
			os << value;
			return os.str();
		}
};
