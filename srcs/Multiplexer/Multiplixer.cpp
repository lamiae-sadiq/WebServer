/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Multiplixer.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kel-baam <kel-baam@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/21 09:31:07 by kel-baam          #+#    #+#             */
/*   Updated: 2024/03/07 20:24:15 by kel-baam         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/Multiplixer.hpp"
#include "../../includes/response.hpp"
#include <algorithm>

#include <csignal>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
void setNonBlocking(int &sockfd) {
    int flags = fcntl(sockfd, F_GETFL, 0);
    if (flags == -1) {
        perror("fcntl");
        exit(EXIT_FAILURE);
    }
    if (fcntl(sockfd, F_SETFL, flags | O_NONBLOCK) == -1) {
        perror("fcntl");
        exit(EXIT_FAILURE);
    }
}
void Multiplixer::add_event(int epo,int sockfd,epoll_event *event, int flag)
{
	// setNonBlocking(sockfd);
	if(!flag)
		event->events = EPOLLIN;
	else
		event->events = EPOLLIN | EPOLLOUT ;
	event->data.fd = sockfd;
	int epo_ctl= epoll_ctl(epo,EPOLL_CTL_ADD,sockfd,event);
	if(epo_ctl < 0)
		throw networkError();
}

void Multiplixer::bindSocket(int fdSocket,int port,Server server)
{
	struct sockaddr_in address;
	struct addrinfo info,*res,*p;
	(void)server;
    memset(&info, 0, sizeof info);
	info.ai_family = AF_INET;
	int status = getaddrinfo(server.getServerData("host")[0].c_str(),NULL,&info,&res);
	if(status != 0)
	{
		fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(status));
		exit(0);
	}
	for(p = res;p!=NULL;p=p->ai_next)
	{
		if (p->ai_family == AF_INET)
		{
			address = (*(struct sockaddr_in *)p->ai_addr);
			printf("IPv4 Address: %s\n", inet_ntoa(address.sin_addr));
			break;
		}
	}
	address.sin_family = AF_INET; 
	address.sin_port = htons(port);
	int bb= bind(fdSocket, (struct sockaddr *)&address,sizeof(address)) ;
	if( bb < 0)
	{
		throw networkError();
	}
}

int Multiplixer::checkMasterSocketPort(Server server)
{
	std::map<int, std::vector<Server> >::iterator it = masterSockets.begin();
	if(masterSockets.size() > 0)
	{
		for(it = masterSockets.begin(); it != masterSockets.end();it++)
		{
			for(size_t i = 0; i < it->second.size()  ;i++)
			{
				if((it->second)[i].getServerData("port")[0] == server.getServerData("port")[0] &&
				 (it->second[i]).getServerData("host")[0] == server.getServerData("host")[0])
				 {
					return it->first;
				 }
			}
		}
	}
	return -1;
}

void Multiplixer::creatSockets(int epo,std::vector<Server> servers)
{
	int listenn;
	epoll_event event;
	int masterSockfd;
	int a = 1;
	for(size_t i = 0; i < servers.size();i++)
	{
		//TODO check servers that have same names AND SAME PORT
		masterSockfd = checkMasterSocketPort(servers[i]);
		if(masterSockfd == -1)
		{	
			masterSockfd = socket(AF_INET,SOCK_STREAM,0);
			if(masterSockfd < 0)
				throw networkError();
			if (setsockopt(masterSockfd, SOL_SOCKET, SO_REUSEADDR,&a, sizeof(int)) < 0)
    			perror("setsockopt(SO_REUSEADDR) failed");
			bindSocket(masterSockfd,atoi(servers[i].getServerData("port")[0].c_str()),servers[i]);
			listenn = listen(masterSockfd,2);
			printf("listiining......\n");
			add_event(epo,masterSockfd,&event,0);
		}
		masterSockets[masterSockfd].push_back(servers[i]);
	}
}

void Multiplixer::CreateNetwork(int &epo,std::vector<Server> servers)
{
	epo = epoll_create(100);
	if(epo < 0)
		throw networkError();
	//delete epo
	creatSockets(epo,servers);
}

void  Multiplixer::acceptNewConnection(int epo,int sockfd,epoll_event *events)
{
	int newSockfd;
	struct sockaddr_in client_add;
	socklen_t add_len = sizeof(client_add);
	
	newSockfd = accept(sockfd,(struct sockaddr *)&client_add,&add_len);	
	//protectio
	matchServers[newSockfd] = masterSockets[sockfd];
	add_event(epo,newSockfd,events,1);
	requests[newSockfd] = new Request();
	Request &request = *(requests[newSockfd]);
	responses[newSockfd] = new response(request);

}

void Multiplixer::clearSocketFdFRomEpoll(int socketFd,int epo,struct  epoll_event *events,int index)
{
	if(epoll_ctl(epo,EPOLL_CTL_DEL,socketFd, &events[index]) < 0)
									throw networkError();
	close(socketFd);
	std::cout << "respone sent to  "  << socketFd << "|||is done" << std::endl;
	delete (responses[socketFd]);
	delete (requests[socketFd]);
	responses.erase(socketFd);
	requests.erase(socketFd);
}

void Multiplixer::start(std::vector<Server> servers)
{
	epoll_event events[MAX_EVENTS];
	size_t readyFiles;
	int epo;
	char buff[1024];
	
	int code;
	int byt;
	CreateNetwork(epo,servers);
	signal(SIGPIPE, SIG_IGN);
	while(1)
	{
		readyFiles = epoll_wait(epo,events,MAX_EVENTS,-1);
		for(size_t i = 0; i < readyFiles;++i)
		{
			bzero(buff,sizeof(buff));
			byt = 0;
			if((masterSockets.count(events[i].data.fd)) && (events[i].events & EPOLLIN))
				acceptNewConnection(epo, events[i].data.fd,events);
			else
			{
				int socketFd = events[i].data.fd;
				Request &request = *(requests[socketFd]);
				if((events[i].events & EPOLLIN))
				{
					byt = recv(socketFd,buff,1024, 0);
					if(byt <= 0)
					{
						clearSocketFdFRomEpoll(socketFd, epo, events,i);
						continue;
					}
					code = request.parseHeaders(std::string(buff,byt),matchServers[socketFd]);
					if(code!=1)
					{
						responses[socketFd]->setStatusCode(code);
					}
				}
				if((events[i].events & EPOLLOUT))
				{
					if(request.getStatus() == 1)
					{
						// std::cout << "start response\n";
						responses[socketFd]->executeMethodes(buff,byt,socketFd);
						if(responses[socketFd]->getFlag() == 30 || responses[socketFd]->getFlag() == 201)
							clearSocketFdFRomEpoll(socketFd, epo, events,i);
					}
				}
			}
		}
	}
}


