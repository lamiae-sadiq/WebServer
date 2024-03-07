/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Multiplixer.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kel-baam <kel-baam@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/21 09:31:05 by kel-baam          #+#    #+#             */
/*   Updated: 2024/02/24 15:24:20 by kel-baam         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <string.h>
#include <iostream>
#include <sys/epoll.h>
#include <fcntl.h> 
#include <errno.h>
#include <vector>
#include "./Utils.hpp"
#include <map>
#include "./response.hpp"
#include "./Request.hpp"
#define MAX_EVENTS 50
// #define PORT 9000

static std::string tmpBuff;
class Multiplixer
{
		int count;
		// response res;
		std::map<int,std::vector<Server> > masterSockets;
		std::map<int,std::vector<Server> > matchServers ;
		std::map<int,response *> responses;
		std::map<int,Request*>  requests;
	public:
		Multiplixer()
		{
			tmpBuff="";
			count = 0;
		}
		~Multiplixer(){};
		int checkMasterSocketPort(Server server);
		void analyseRequest(char * buff, int &completeRead);
		void add_event(int epo,int sockfd,epoll_event *event, int flag);
		void creatSockets(int epo,std::vector<Server> servers);
		void bindSocket(int fdSocket,int port,Server server);
		void CreateNetwork(int &epo,std::vector<Server> srevers);
		void readReqeust(int &fd,int &completeRead);
		void start(std::vector<Server> servers);
		void storeRequest(std::string line);
		void storeRequestLineInfo(std::vector<std::string> vec);
		void clearSocketFdFRomEpoll(int socketFd,int epo,struct  epoll_event *events,int index);		
		void acceptNewConnection(int epo,int sockfd,epoll_event *events);
	  	class networkError:public std::exception 
        {
            const char* what() const throw()
            {
                return "you have error on your network\n";
            }
        };
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

