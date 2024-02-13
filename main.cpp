/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lsadiq <lsadiq@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/02 14:45:23 by lsadiq            #+#    #+#             */
/*   Updated: 2024/02/12 20:09:39 by lsadiq           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./includes/webs.hpp"


int main()
{
    location::fillMime();
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
            location loc;
            loc.fd = accept(sockfd, (struct sockaddr *)&address, (socklen_t *)&addrlen);
            if (loc.fd < 0)
                throw std::runtime_error("Error: didn't accept");
            char c[1000];
            read(loc.fd, c, 1000);
            while (loc.close)
            {
                loc.methodGet();
            }

            // std::ifstream videoFile(loc.newlocation, std::ios::binary);
            // if (!videoFile.is_open())
            //     throw std::runtime_error("Error: failed to open video file");
            // std::string resHeader = "HTTP/1.1 " + std::to_string(loc.status_code) + " OK\r\n";
            // if (loc.status_code == 301)
            //     resHeader += "Location: " + loc.newlocation + "\r\n\r\n";
            // else
            // {
            //     // std::string resHeader = "HTTP/1.1 200 OK\r\n";
            //     resHeader += "Content-Type: ";
            //     resHeader += loc.content_type;
            //     std::cout<< loc.content_type << std::endl;
            //     resHeader +="\r\n";
            //     resHeader += "Transfer-Encoding: chunked\r\n";
            //     resHeader += "\r\n";
            // }
            // send(clientsocket, resHeader.c_str(), resHeader.length(), 0);
            // const int chunkSize = 1024;
            // char buffer[chunkSize];
            // while (!videoFile.eof())
            // {
            //     videoFile.read(buffer, chunkSize);
            //     int bytesRead = videoFile.gcount();
            //     if (bytesRead > 0)
            //     {
            //         std::stringstream ss;
            //         ss << std::hex << bytesRead;
            //         std::string chunkSizeHex = ss.str();
            //         std::string chunkHeader = chunkSizeHex + "\r\n";
            //         send(clientsocket, chunkHeader.c_str(), chunkHeader.length(), 0);
            //         send(clientsocket, buffer, bytesRead, 0);
            //         send(clientsocket, "\r\n", 2, 0);
            //     }
            // }
            // send(clientsocket, "0\r\n\r\n", 5, 0);
            // videoFile.close();
            close(loc.fd);
        }
        close(sockfd);
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << std::endl;
    }
    return 0;
}