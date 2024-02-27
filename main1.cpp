/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lsadiq <lsadiq@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/02 14:45:23 by lsadiq            #+#    #+#             */
/*   Updated: 2024/02/23 16:30:46 by lsadiq           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./includes/response.hpp"


int main()
{
    response::fillMime();
    try
    {
        signal(SIGPIPE, SIG_IGN);
        int opt = 1;
        int sockfd = socket(AF_INET, SOCK_STREAM, 0);
        if (sockfd < 0)
            throw std::runtime_error("Error: socket creation failed");
        struct sockaddr_in address;
        memset(&address, 0, sizeof(address));
        if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, (const char *)&opt, sizeof(opt)))
        {
            perror("setsockopt");
            exit(EXIT_FAILURE);
        }
        address.sin_family = AF_INET;
        address.sin_addr.s_addr = INADDR_ANY;
        address.sin_port = htons(PORT);
        int addrlen = sizeof(address);
        if (bind(sockfd, (struct sockaddr *)&address, sizeof(address)) < 0)
            throw std::runtime_error("Error: bind failed");
        if (listen(sockfd, 10) < 0)
            throw std::runtime_error("Error: listen failed");
        std::cout << "Server listening on port " << PORT << std::endl;
        while (1)
        {
            Request *req = new Request();
            response locc(*req);
            // std::cout << loc.getFd() << std::endl;
            std::cout <<"DDddd\n";
            locc.setFd(accept(sockfd, (struct sockaddr *)&address, (socklen_t *)&addrlen));
            if (locc.getFd() < 0)
                throw std::runtime_error("Error: didn't accept");
            while (locc.getClose())
            {
                 if (locc.getStatusCode() != 200)
                    locc.methodGet();
                 else
                    locc.methodPost("sss");
            }
            close(locc.getFd());
        }
        close(sockfd);
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << std::endl;
    }
    return 0;
}