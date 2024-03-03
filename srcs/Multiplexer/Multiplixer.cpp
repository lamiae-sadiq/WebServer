/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Multiplixer.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lsadiq <lsadiq@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/21 09:31:07 by kel-baam          #+#    #+#             */
/*   Updated: 2024/03/02 17:13:58 by lsadiq           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/Multiplixer.hpp"
#include "../../includes/response.hpp"
#include <algorithm>

#include <csignal>

void Multiplixer::add_event(int epo,int sockfd,epoll_event *event, int flag)
{
	if(!flag)
		event->events = EPOLLIN;
	else
		event->events = EPOLLIN | EPOLLOUT;
	event->data.fd = sockfd;
	int epo_ctl= epoll_ctl(epo,EPOLL_CTL_ADD,sockfd,event);
	if(epo_ctl < 0)
		throw networkError();
}

void Multiplixer::bindSocket(int fdSocket,int port)
{
	struct sockaddr_in address;
	
	address.sin_family = AF_INET; 
	address.sin_addr.s_addr = INADDR_ANY;
	address.sin_port = htons(port);
	if(bind(fdSocket, (struct sockaddr *)&address,sizeof(address)) < 0)
		throw networkError();
}

int Multiplixer::checkMasterSocketPort(Server server)
{
	std::map<int, std::vector<Server> >::iterator it = masterSockets.begin();
	if(masterSockets.size() > 0)
	{
		for(it = masterSockets.begin(); it != masterSockets.end();it++)
		{
			for(size_t i = 0; i <it->second.size()  ;i++)
			{
				if((it->second)[i].getServerData("port")[0] == server.getServerData("port")[0])
					return it->first;
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
	for(size_t i = 0; i <servers.size();i++)
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
			bindSocket(masterSockfd,atoi(servers[i].getServerData("port")[0].c_str()));
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


int Multiplixer::acceptNewConnection(int epo,int sockfd,epoll_event *events)
{
	int newSockfd;
	struct sockaddr_in client_add;
	socklen_t add_len = sizeof(client_add);
	
	newSockfd = accept(sockfd,(struct sockaddr *)&client_add,&add_len);	
	matchServers[newSockfd] = masterSockets[sockfd];
	add_event(epo,newSockfd,events,1);
	return newSockfd;
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
	while(1)
	{
		signal(SIGPIPE, SIG_IGN);
		readyFiles = epoll_wait(epo,events,MAX_EVENTS,-1);

		for(size_t i = 0; i < readyFiles;++i)
		{
			if((masterSockets.count(events[i].data.fd)) && (events[i].events & EPOLLIN))
			{
				int newSocket = acceptNewConnection(epo, events[i].data.fd,events);
				requests[newSocket] = new Request();
				Request &request = *(requests[newSocket]);
				responses[newSocket] = new response(request);
			}
			else
			{
				int socketFd = events[i].data.fd;
				Request &request = *(requests[socketFd]);
				if((events[i].events & EPOLLIN ))
				{
					byt = read(socketFd,buff,1023);
					code = request.parseHeaders(std::string(buff,byt),matchServers[socketFd]);
				
					if(code!=1)
					{	
						std::cout <<"mult"<< code << "\n";
						responses[socketFd]->setStatusCode(code);
					}
				}
				if((events[i].events & EPOLLOUT))
				{
					if(request.getStatus() == 1)
					{
							responses[socketFd]->executeMethodes(buff,byt,socketFd);
							if(responses[socketFd]->getFlag() == 30 || responses[socketFd]->getFlag() == 201)
							{
								if(epoll_ctl(epo,EPOLL_CTL_DEL,socketFd, &events[i]) < 0)
									throw networkError();
								close(socketFd);
								std::cout << "respone sent to  " << socketFd << "is done" << std::endl;
								delete (responses[socketFd]);
								delete (requests[socketFd]);
								responses.erase(socketFd);
								requests.erase(socketFd);
								// return;
							}
					}
				}
			}
		}
	}
	//close master sockets
	// close(sockfd);
}


