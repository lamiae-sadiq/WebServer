/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Multiplixer.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kel-baam <kel-baam@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/21 09:31:05 by kel-baam          #+#    #+#             */
/*   Updated: 2024/03/20 22:32:45 by kel-baam         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <unistd.h>
#include <string.h>
#include <iostream>
#include <sys/epoll.h>
#include <fcntl.h> 
#include <errno.h>
#include <vector>
#include <algorithm>
#include <csignal>
#include <sys/types.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <sys/time.h>
#include <map>
#include "./Utils.hpp"
#include "./response.hpp"
#include "./Request.hpp"
#include "Exception.hpp"
#define MAX_EVENTS 50
#define EXPECTEDFDS 1000

static std::string tmpBuff;
class Multiplixer
{
		std::map<int,std::vector<Server> > masterSockets;
		std::map<int,std::vector<Server> > matchServers ;
		std::map<int,response *> responses;
		std::map<int,Request*>  requests;
	public:
		Multiplixer();
		~Multiplixer();
		int checkMasterSocketPort(Server server);
		void addFdToEpoll(int epoll_instance,int sockfd,epoll_event *event, int flag);
		void creatSockets(int epo,std::vector<Server> servers);
		void bindSocket(int fdSocket,std::string port,Server server);
		void CreateNetwork(int &epoll_instance,std::vector<Server> srevers);
		void start(std::vector<Server> servers);
		void clearSocketFdFRomEpoll(int socketFd,int epoll_instance,struct  epoll_event *events,int index);		
		void acceptNewConnection(int epoll_instance,int sockfd,epoll_event *events);
		void closeMasterSocket();
        void  restartTime(Request &request);
        bool isTimedout(response *response,Request &request);
        void setNonBlocking(int &sockfd);
};

