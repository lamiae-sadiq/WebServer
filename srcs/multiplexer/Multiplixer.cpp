/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Multiplixer.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kel-baam <kel-baam@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/21 09:31:07 by kel-baam          #+#    #+#             */
/*   Updated: 2024/02/23 15:05:09 by kel-baam         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Multiplixer.hpp"

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
	std::cout << "size" << servers.size() << std::endl;
	for(size_t i = 0; i <servers.size();i++)
	{
		//TODO check servers that have same names AND SAME PORT
		masterSockfd = checkMasterSocketPort(servers[i]);
		if(masterSockfd == -1)
		{			
			masterSockfd = socket(AF_INET,SOCK_STREAM,0);
			if(masterSockfd < 0)
				throw networkError();
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
	int epo_ctl;
	
	epo = epoll_create(100);
	if(epo < 0)
		throw networkError();
	//delete epo
	creatSockets(epo,servers);
}


void Multiplixer::acceptNewConnection(int epo,int sockfd,epoll_event *events)
{
	int newSockfd;
	struct sockaddr_in client_add;
	socklen_t add_len = sizeof(client_add);
	
	newSockfd = accept(sockfd,(struct sockaddr *)&client_add,&add_len);
	std::cout  << "|||" << newSockfd<<  "new conne\n";
	
	matchServers[newSockfd] = masterSockets[sockfd];
	add_event(epo,newSockfd,events,1);
}

void Multiplixer::start(std::vector<Server> servers)
{
	struct sockaddr_in client_add;
	socklen_t add_len = sizeof(client_add);
	epoll_event events[MAX_EVENTS];
	int readyFiles;
	int epo;
	int completeRead = 0;
	
	char buff[1024];
	CreateNetwork(epo,servers);
	while(1)
	{
		readyFiles = epoll_wait(epo,events,MAX_EVENTS,-1);
		
		for(size_t i = 0; i < readyFiles;++i)
		{
			if((masterSockets.count(events[i].data.fd)) && (events[i].events & EPOLLIN))
				acceptNewConnection(epo, events[i].data.fd,events);
			if((events[i].events & EPOLLIN) && !completeRead)
			{
				std::cout << "my file is:" << events[i].data.fd << std::endl;
				completeRead = 1;
				std::cout << buff << std::endl;
				if(requests[events[i].data.fd].parseRequest(events[i].data.fd,completeRead,matchServers[events[i].data.fd]))
				{
					//lamia function should return or change completeRead flag to starrt response
					std::cout << "lamiiiia's part\n";
					std::cout <<"pps:==>>" << requests[events[i].data.fd].tmpBuff<< std::endl;
					completeRead = 1;
				}
			}
			if((events[i].events & EPOLLOUT) && completeRead)
			{
				
				std::cout <<"writing\n";
				std::string content = "<html><body>Hello, world!</body></html>";
				std::string response =
					"HTTP/1.1 200 OK\r\nContent-Type: text/html\r\nContent-Length: 39\r\n\r\n" + content;
				send(events[i].data.fd,response.c_str(),response.length(),0);
				 if(epoll_ctl(epo,EPOLL_CTL_DEL,events[i].data.fd,&events[i]) < 0)
				 	throw networkError();
				close(events[i].data.fd);
			}
		}
	}
	//close master sockets
	// close(sockfd);
}