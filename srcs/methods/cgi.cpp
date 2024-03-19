/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cgi.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kel-baam <kel-baam@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/07 03:01:02 by lsadiq            #+#    #+#             */
/*   Updated: 2024/03/19 15:46:47 by kel-baam         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/response.hpp"
#include "../../includes/Request.hpp"
#include "sys/wait.h"
#include <fcntl.h>
#include <sys/stat.h>

void response::setEnv()
{
	std::string cgiEnv;
	cgiHeader["CONTENT_TYPE"] = request.getContentType();
	if (request.getMethod() == "POST")
	{
		struct stat datafile;
		stat(uplfile.c_str(), &datafile);
		cgiHeader["CONTENT_LENGTH"] = to_string(datafile.st_size);
	}
	cgiHeader["GATEWAY_INTERFACE"] = "CGI";
	cgiHeader["REQUEST_METHOD"] = request.getMethod();
	
	cgiHeader["QUERY_STRING"] = request.getQueryString();
	cgiHeader["PATH_INFO"] = request.getUrl();
	cgiHeader["HTTP_COOKIE"] = request.getCookies();
	// std::cout <<"coookies"<< cgiHeader["HTTP_COOKIE"] << "=======" <<  request.getCookies() <<"\n";
	cgiHeader["SCRIPT_NAME"] = request.getUrl();
	cgiHeader["SCRIPT_FILENAME"] = targetUri;
	cgiHeader["PATH_TRANSLATED"] = targetUri;
	cgiHeader["SERVER_PROTOCOL"] = "HTTP/1.1";
	cgiHeader["REDIRECT_STATUS"] = "200";

	env = new char *[cgiHeader.size() + 1];
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

char **response::getEnv()
{
	int i = 0;
	while (env[i] != NULL)
	{
		std::cout << env[i] << std::endl;
		i++;
	}
	return env;
}

void response::executePHP()
{
	setEnv();
	std::cout << "___________PHP_______\n";
	std::map<std::string, std::string>::iterator it = request.location.cgi.begin();
	it = request.location.cgi.find("php");
	if (it != request.location.cgi.end())
	{
		cgiStartTime = time(NULL);
		std::string randName = generateName();
		path = "/nfs/homes/kel-baam/Desktop/newOne/" + randName;
		pid = fork();
		if (pid == 0)
		{
			std::cout << "=================================================pop = " << request.getMethod() << std::endl;
			if (request.getMethod() == "POST")
			{
				freopen(uplfile.c_str(), "r", stdin);
				remove(uplfile.c_str());
			}
			freopen(path.c_str(), "w", stdout);
			char *av[3];
			av[0] = strdup(it->second.c_str());
			av[1] = strdup(targetUri.c_str());
			av[2] = NULL;
			execve(it->second.c_str(), av, env);
			exit(0);
		}
		else
		{
			_cgiStarted = true;
		}
	}
}

bool response::_cgiProcess()
{
	if (_cgiEnded)
		return true;
	int status;
	if (pid < 0)
	{
		status_code = 500;
		_cgiEnded = true;
		return true;
	}
	else
	{
		int wait = waitpid(pid, &status, WNOHANG);
		if (wait == 0)
		{
			long long now = time(NULL);

			request.setCGIRun();
			if ((now -  cgiStartTime) > 10)
			{
				std::cout << "time out \n";
				kill(pid, SIGKILL);
				status_code = 504;
				_cgiEnded = true;
				return true;
			}
			else
				return false;
		}
		else if (wait > 0)
		{
			if (WIFEXITED(status))
				status_code = 200;
			_cgiEnded = true;
			// std::cout << "___________CGIEnded_______1\n";
			return true;
		}
		else
		{
			std::cout << "internel server "<< std::endl;
			status_code = 500;
			_cgiEnded = true;
			return true;
		}
	}
	return false;
}

void response::handelCGI()
{
	if (!_cgiStarted)
	{
		if (request.location.cgi.find("php") != request.location.cgi.end())
			executePHP();
		else
			executePython();
	}
	if (!_cgiProcess())
		return;
	if (_cgiEnded)
	{
		// std::cout << "___________CGIHandeler_______11111111111 \n";
		if (status_code == 500)
		{
			remove(path.c_str());
			status_code = 500;
		}
		else if (status_code == 504)
		{
			remove(path.c_str());
			status_code = 504;
		}
		else if (status_code == 408)
		{
			remove(path.c_str());
			status_code = 408;
		}
		else
		{
			// std::cout << "___________CGIEnded_______\n";
			cgiSendResponse();
			return;
		}
	}
}

void response::executePython()
{
	std::cout << "___________PY_______\n";
	std::map<std::string, std::string>::iterator it = request.location.cgi.begin();
	it = request.location.cgi.find("py");
	if (it != request.location.cgi.end())
	{
		setEnv();/////////////////////////////////////////////////////////////////////////////////
		cgiStartTime = time(NULL);
		std::string randName = generateName();
		path = "/nfs/sgoinfre/goinfre/Perso/lsadiq/last/" + randName;
		pid = fork();
		if (pid == 0)
		{
			std::cout << "=================================================pop = " << request.getMethod() << std::endl;
			if (request.getMethod() == "POST")
			{
				freopen(uplfile.c_str(), "r", stdin);
				remove(uplfile.c_str());
			}
			freopen(path.c_str(), "w", stdout);
			char *av[3];
			av[0] = strdup(it->second.c_str());
			av[1] = strdup(targetUri.c_str());
			av[2] = NULL;
			execve(it->second.c_str(), av, env);
			exit(0);
		}
		else
		{
			_cgiStarted = true;
		}
	}
}

void response::parsecgiFile()
{
	cinfile.open(path.c_str());
	if (cinfile.is_open())
	{
		std::string line;
		while (getline(cinfile, line))
		{
			if (line == "\n" || line == "\r\n" || line == "\r" || line == "")
				break;
			size_t pos = line.find(':');
			if (pos != std::string::npos)
			{
				std::string key = line.substr(0, pos);
				std::string value = line.substr(pos + 1);
				_cgiHeader[key] = value;
			}
		}
	}
	else
		status_code = 404;
}

void response::cgiSendResponse()
{
	_isStatus = false ;
	parsecgiFile();
	std::string resHeader = "HTTP/1.1 ";
	if(_cgiHeader["Status"].empty())
		resHeader += to_string(status_code) + " " + setStatus(status_code);
	else
		resHeader += _cgiHeader["Status"];
	resHeader += "\r\nContent-Type:";
	if(!_cgiHeader["Content-Type"].empty() || !_cgiHeader["Content-type"].empty())
	{
		if (!_cgiHeader["Content-Type"].empty())
			resHeader += _cgiHeader["Content-Type"];
		else
			resHeader += _cgiHeader["Content-type"];
	}
	resHeader += "\r\n";
	if (!_cgiHeader["Location"].empty()){
		resHeader += "Location:";
		resHeader += _cgiHeader["Location"] + "\r\n";
	}
	resHeader += "Transfer-Encoding: chunked\r\n";
	if(_cgiHeader.find("Set-Cookie")!=_cgiHeader.end())
		resHeader += "Set-Cookie:" + _cgiHeader["Set-Cookie"] + "\r\n";
	resHeader += "\r\n";
	// std::cout << resHeader;
	int i = send(fd, resHeader.c_str(), resHeader.length(), 0);
	if(i < 0){
		SIGPIPE;
		flag = 30;
	}
	flag = 3;
	if (flag == 3)
	{
		const int chunkSize = 1024;
		char buffer[chunkSize];
		cinfile.read(buffer, chunkSize);
		int bytesRead = cinfile.gcount();
		std::stringstream ss;
		ss << std::hex << bytesRead;
		std::string chunkSizeHex = ss.str();
		std::string chunkHeader = chunkSizeHex + "\r\n";
		char concatenatedSends[chunkHeader.length() + bytesRead + 2];
		memcpy(concatenatedSends, chunkHeader.c_str(), chunkHeader.length());
		memcpy(concatenatedSends + chunkHeader.length(), buffer, bytesRead);
		memcpy(concatenatedSends + chunkHeader.length() + bytesRead, "\r\n", 2);
		int i = write (fd, concatenatedSends, chunkHeader.length() + bytesRead + 2);
		if(i < 0){
			SIGPIPE;
			flag = 30;
		}
		if (cinfile.eof()){
			flag = 4;
		}
	}
	if (flag == 4) {
		int i = send(fd, "0\r\n\r\n", 5, 0);
		if(i < 0){
			SIGPIPE;
			flag = 30;
		}
		remove(path.c_str());
		cinfile.close();
		flag = 30;
	}
}