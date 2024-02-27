/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Multiplixer.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kel-baam <kel-baam@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/21 09:31:07 by kel-baam          #+#    #+#             */
/*   Updated: 2024/02/24 18:50:43 by kel-baam         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/Multiplixer.hpp"
#include "../../includes/response.hpp"
#include <algorithm>
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
	
	std::cout << port << "|||" << fdSocket << std::endl; 
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
	std::cout  << "|||" << newSockfd<<  "new conne\n";
	
	matchServers[newSockfd] = masterSockets[sockfd];
	add_event(epo,newSockfd,events,1);
	return newSockfd;
}

void Multiplixer::start(std::vector<Server> servers)
{
// 	struct sockaddr_in client_add;
	// socklen_t add_len = sizeof(client_add);
	epoll_event events[MAX_EVENTS];
	size_t readyFiles;
	int epo;
	char buff[1024];
	
	Request req;
	CreateNetwork(epo,servers);
	while(1)
	{
		readyFiles = epoll_wait(epo,events,MAX_EVENTS,-1);

		for(size_t i = 0; i < readyFiles;++i)
		{
			// std::cout << readyFiles << "\n";
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
				if((events[i].events & EPOLLIN ) && !request.getStatus())
				{
					int byt = read(socketFd,buff,1024);
					buff[byt] = '\0';
					if(request.parseHeaders(buff,matchServers[socketFd]))
					{
						// if(!request.tmpBuff.empty())
						// {
							//responses[socketFd]->methodPost(request.tmpBuff,request.tmpBuff.length());
							// std::cout << tmpBuff <<"\n";
						// 	tmpBuff = "";
						// }
						//else buff
							// responses[socketFd]->methodPost(buff,byt,index);
							//request.getStatus() in the end of response
						std::cout << "request from " << socketFd << "is done\n";
					}
				}
				if((events[i].events & EPOLLOUT) &&  request.getStatus() == 1)
				{
					responses[socketFd]->setFd(socketFd);
					responses[socketFd]->methodGet();
					if(responses[socketFd]->getFlag() == 30)
					{
						if(epoll_ctl(epo,EPOLL_CTL_DEL,socketFd, &events[i]) < 0)
							throw networkError();
						close(socketFd);
						std::cout << "respone sent to  " << socketFd << "is done" << std::endl;
						responses.erase(socketFd);
						requests.erase(socketFd);
					}
				}
			}
		}
	}
	//close master sockets
	// close(sockfd);
}


