/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cgi.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lsadiq <lsadiq@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/07 03:01:02 by lsadiq            #+#    #+#             */
/*   Updated: 2024/03/11 18:25:08 by lsadiq           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// #include "../../includes/cgi.hpp"
#include "../../includes/response.hpp"
#include "../../includes/Request.hpp"
#include "sys/wait.h"
#include <fcntl.h>

void response::setEnv()
{
	// Set environment variables for CGI
	
	std::string cgiEnv;
	cgiHeader["CONTENT_TYPE"] = request.getContentType();
	cgiHeader["CONTENT_LENGTH"] = request.getContentLength();
	cgiHeader["GATEWAY_INTERFACE"] = "CGI";
	cgiHeader["REQUEST_METHOD"] = request.getMethod();
	cgiHeader["QUERY_STRING"] = request.getQueryString();
	cgiHeader["PATH_INFO"] = request.getUrl();
	cgiHeader["HTTP_COOKIE"] = request.getCookies();
	cgiHeader["SCRIPT_NAME"] = request.getUri();
	cgiHeader["PATH_TRANSLATED"] = request.getUri();
	cgiHeader["SERVER_PROTOCOL"] = "HTTP/1.1";
	cgiHeader["REDIRECT_STATUS"] = "200";
	
	env = new char* [cgiHeader.size() + 1];
	int i = 0;
	for (std::map<std::string, std::string>::iterator it = cgiHeader.begin(); it != cgiHeader.end(); ++it)
	{
		std::string envVar = it->first + "=" + it->second;
		env[i] = new char[envVar.length() + 1];
		strcpy(env[i], envVar.c_str());
		i++;
	}
	env[i] = NULL;
}


char** response::getEnv()
{
	int i = 0;
	while (env[i] != NULL)
	{
		std::cout << env[i] << std::endl;
		i++;
	}
	return env;
}

void response::executePHP(std::string &file)
{
	(void)file;
	std::cout << "___________PHP_______\n";
	std::map<std::string, std::string>::iterator it = request.location.cgi.begin();
	it = request.location.cgi.find("php");
	if (it != request.location.cgi.end())
	{
		std::string randName = generateName();
		std::string path = "/nfs/sgoinfre/goinfre/Perso/lsadiq/webserv/" + randName;
		cgiOutfile = open(path.c_str(), O_CREAT | O_RDWR, 0666);
		close(cgiOutfile);
		if (cgiOutfile < 0)
		{
			status_code = 403;
			return ;
		}
		pid = fork();
		if(pid == 0)
		{
			setEnv();
			dup2(cgiOutfile, 1);
			// if (is post)
			// freopen(postpath.c_str(), "r", stdout);
			freopen(path.c_str(), "w", stdout);
			close(cgiOutfile);
			char *av[3];
			av[0] = strdup(it->second.c_str());
			av[1] = strdup(targetUri.c_str());
			av[2] = NULL;
			execve(it->second.c_str(), av, NULL);
			exit(0); 
		}
		else
		{
			waitpid( pid, NULL, WNOHANG );
		}
	}
}


void response::executePython(std::string &file)
{
	(void)file;
	std::cout << "___________PY_______\n";
	std::map<std::string, std::string>::iterator it = request.location.cgi.begin();
	it = request.location.cgi.find("py");
	if (it != request.location.cgi.end())
	{
		std::string randName = generateName();
		std::string path = "/nfs/sgoinfre/goinfre/Perso/lsadiq/webserv/" + randName;
		// cgiOutfile = open(path.c_str(), O_CREAT | O_RDWR, 0666);
		// close(cgiOutfile);
		// if (cgiOutfile < 0)
		// {
		// 	status_code = 403;
		// 	return ;
		// }
		pid = fork();
		if(pid == 0)
		{
			setEnv();
			// dup2(cgiOutfile, 1);
			// if (is post)
			// freopen(postpath.c_str(), "r", stdout);
			freopen(path.c_str(), "w", stdout);
			// close(cgiOutfile);
			char *av[3];
			av[0] = strdup(it->second.c_str());
			av[1] = strdup(targetUri.c_str());
			av[2] = NULL;
			execve(it->second.c_str(), av, NULL);
			exit(0); 
		}
		else
		{
			waitpid( pid, NULL, WNOHANG );
		}
	}
}