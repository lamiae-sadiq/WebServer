/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Multiplixer.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lsadiq <lsadiq@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/21 09:31:07 by kel-baam          #+#    #+#             */
/*   Updated: 2024/03/23 21:38:14 by lsadiq           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/Multiplixer.hpp"

Multiplixer::Multiplixer()
{
	tmpBuff = "";
}

Multiplixer::~Multiplixer(){};

void Multiplixer::setNonBlocking(int &sockfd) 
{
    if(fcntl(sockfd, F_SETFL, O_NONBLOCK, FD_CLOEXEC)== -1)
        throw Exception(strerror(errno));
}

void Multiplixer::addFdToEpoll(int epoll_instance,int sockfd, epoll_event *event, int flag)
{
	setNonBlocking(sockfd);
	if(!flag)
		event->events = EPOLLIN;
	else
		event->events = EPOLLIN | EPOLLOUT;
	event->data.fd = sockfd;
	int epo_ctl= epoll_ctl(epoll_instance,EPOLL_CTL_ADD,sockfd,event);
	if(epo_ctl < 0)
	{
		closeMasterSocket();
        throw Exception(strerror(errno));
	}
}

void Multiplixer::bindSocket(int fdSocket,std::string port,Server server)
{
	struct sockaddr_in address;
	struct addrinfo info,*listOfAdresses,*p;
	
    memset(&info, 0, sizeof(info));
	info.ai_family = AF_INET;
	int getAddressInfo = getaddrinfo(server.getServerData("host")[0].c_str(),port.c_str(),&info,&listOfAdresses);
	if(getAddressInfo != 0)
	{
		closeMasterSocket();
        throw Exception(gai_strerror(getAddressInfo));
	}
	for(p = listOfAdresses; p!=NULL; p=p->ai_next)
	{
		if (p->ai_family == AF_INET)
		{
			address = (*(struct sockaddr_in *)p->ai_addr);
			break;
		}
	}
	freeaddrinfo(listOfAdresses);
	address.sin_family = AF_INET; 
	int bindStatus = bind(fdSocket, (struct sockaddr *)&address,sizeof(address));
	if( bindStatus < 0)
	{
		closeMasterSocket();
        throw Exception("bind error\n");
	}
}

void Multiplixer::closeMasterSocket()
{
	std::map<int, std::vector<Server> >::iterator it = masterSockets.begin();
	for(it = masterSockets.begin(); it != masterSockets.end();it++)
		close(it->first);
}

int Multiplixer::checkMasterSocketPort(Server server)
{
	std::map<int, std::vector<Server> >::iterator it = masterSockets.begin();
	int isValid = 0;
	std::string port;
	std::string host;
	if(masterSockets.size() > 0)
	{
		for(it = masterSockets.begin(); it != masterSockets.end();it++)
		{
		
			for(size_t i = 0; i < it->second.size()  ;i++)
			{	
				port = (it->second)[i].getServerData("port")[0];
				host = (it->second[i]).getServerData("host")[0];
				if(port == server.getServerData("port")[0] && host  == server.getServerData("host")[0])
				{
					if((!(it->second[i]).getServerData("server_name").size() && !server.getServerData("server_name").size()) ||
						((it->second[i]).getServerData("server_name")[0] == server.getServerData("server_name")[0]))
					{
						closeMasterSocket();
						throw Exception("Error: you have a servers with same configuration\n");
					}
					isValid = 1;
				}
			}
			if(isValid)
				return it->first;
		}
	}
	return -1;
}

void Multiplixer::creatSockets(int epoll_instance,std::vector<Server> servers)
{
	int listenn;
	epoll_event event;
	int masterSockfd;
	int a = 1;
	for(size_t i = 0; i < servers.size();i++)
	{
		masterSockfd = checkMasterSocketPort(servers[i]);
		if(masterSockfd == -1)
		{	
			masterSockfd = socket(AF_INET,SOCK_STREAM,0);
			if(masterSockfd < 0)
			{
				closeMasterSocket();
        		throw Exception(strerror(errno));
			}
			if (setsockopt(masterSockfd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT ,&a, sizeof(int)) < 0)
			{
				closeMasterSocket();
        		throw Exception(strerror(errno));
			}
			bindSocket(masterSockfd,servers[i].getServerData("port")[0],servers[i]);
			listenn = listen(masterSockfd,SOMAXCONN);
			if(listenn < 0)
			{
				closeMasterSocket();
        		throw Exception(strerror(errno));
			}
			printf("listening......\n");
			addFdToEpoll(epoll_instance,masterSockfd,&event,0);
		}
		masterSockets[masterSockfd].push_back(servers[i]);
	}
}

void Multiplixer::CreateNetwork(int &epoll_instance,std::vector<Server> servers)
{
	epoll_instance = epoll_create(EXPECTEDFDS);
	if(epoll_instance < 0)
	{
		close(epoll_instance);
        throw Exception(strerror(errno));
	}
	creatSockets(epoll_instance,servers);
}

void  Multiplixer::acceptNewConnection(int epoll_instance,int sockfd,epoll_event *events)
{
	int newSockfd;
	struct sockaddr_in client_add;
	socklen_t add_len = sizeof(client_add);
	newSockfd = accept(sockfd,(struct sockaddr *)&client_add,&add_len);
	if(newSockfd < 0)
        throw Exception(strerror(errno));
	matchServers[newSockfd] = masterSockets[sockfd];
	addFdToEpoll(epoll_instance,newSockfd,events,1);
	requests[newSockfd] = new Request();
	Request &request = *(requests[newSockfd]);
	responses[newSockfd] = new response(request);
    responses[newSockfd]->setFd(newSockfd);
	gettimeofday(&request.start_time, NULL);
}

void Multiplixer::clearSocketFdFRomEpoll(int socketFd,int epoll_instance,struct  epoll_event *events,int index)
{
	if(epoll_ctl(epoll_instance,EPOLL_CTL_DEL,socketFd, &events[index]) < 0)
        throw Exception(strerror(errno));
	close(socketFd);
	response *tmp_res = responses[socketFd];
	Request *tmp_req = requests[socketFd];
	responses.erase(socketFd);
	requests.erase(socketFd);
	matchServers.erase(socketFd);
	delete(tmp_req);
	delete(tmp_res);
}

void  Multiplixer::restartTime(Request &request)
{
	request.start_time.tv_sec = 0;
    request.start_time.tv_usec = 0;
	request.end_time.tv_sec = 0;
    request.end_time.tv_usec = 0;
	gettimeofday(&request.start_time, NULL);
}

bool Multiplixer::isTimedout(response *response,Request &request)
{
	gettimeofday(&request.end_time, NULL);
	int time = (request.end_time.tv_sec - request.start_time.tv_sec) * 1000000LL +
    (request.end_time.tv_usec - request.start_time.tv_usec);
	if(request.getMatchedLocation())
		request.setStatus(1);
	if( time > 10000000LL && !response->getIsCgi())
	{
		response->setStatusCode(408);
		request.setStatus(1);
		return true;
	}
	if(request.getMatchedLocation() && request.getMethod() != "POST")
		return true;
	return false;
}

void Multiplixer::start(std::vector<Server> servers)
{
	epoll_event events[MAX_EVENTS];
	size_t readyFiles;
	int epoll_instance;
	char buff[2048];
	int code;
	int byt;
	CreateNetwork(epoll_instance,servers);
	signal(SIGPIPE, SIG_IGN);
	while(1)
	{
		readyFiles = epoll_wait(epoll_instance, events, MAX_EVENTS, -1);
		for(size_t i = 0; i < readyFiles;++i)
		{
			bzero(buff,sizeof(buff));
			byt = 0;
			if((masterSockets.count(events[i].data.fd)) && (events[i].events & EPOLLIN))
				acceptNewConnection(epoll_instance, events[i].data.fd,events);
			else
			{
				int socketFd = events[i].data.fd;
				Request &request = *(requests[socketFd]);
				if(!request.getStatus() && !isTimedout(responses[socketFd],request) && (events[i].events & EPOLLIN))
				{	
					byt = recv(socketFd,buff,2048, 0);
					if(byt <= 0)
					{
						clearSocketFdFRomEpoll(socketFd, epoll_instance, events,i);
						continue;
					}
					request.setSizeBody(byt);
					request.setBody(std::string(buff,byt));
					restartTime(request);
					code = request.parseHeaders(std::string(buff,byt),matchServers[socketFd]);
					if(code != 1)
						responses[socketFd]->setStatusCode(code);
				}
				else if((events[i].events & EPOLLOUT) && request.getStatus() == 1)
				{
					responses[socketFd]->executeMethodes(request.getBody().c_str(),request.getSizeBody());
					request.setStatus(0);
					request.setSizeBody(0);
					request.setBody("");
					if(responses[socketFd]->getClose())
						clearSocketFdFRomEpoll(socketFd, epoll_instance, events,i);					
				}
			}
		}
	}
	closeMasterSocket();
	close(epoll_instance);
}