

#include "./includes/webs.hpp"


int main()
{
    try
    {
        int opt =1;
        int sockfd = socket(AF_INET, SOCK_STREAM, 0);
        if (sockfd < 0)
            throw std::runtime_error("Error: socket creation failed");
        struct sockaddr_in address;
        if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt))) 
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
        std::cout << "Server listening on port "<<  PORT << std::endl;
        while(1)
        {
            int clientsocket = accept(sockfd,(struct sockaddr *)&address, (socklen_t*)&addrlen);
            if (clientsocket < 0 )
                throw std::runtime_error("Error: didnt accept ");
            char buffer[30000] = {0};
            int valread = read( clientsocket , buffer, 30000);
            buffer[valread] = '\0';
            write(1, buffer, valread) ;
            std::string hello = "HTTP/1.1 200 OK\nContent-Type: text/plain\nContent-Length: 19\n\nwa bit gha nghawet!\n";
            send(clientsocket, hello.c_str(), hello.length(), 0);
            std::cout << "Hello message sent" << std::endl;

        close (clientsocket);
        }
        close(sockfd);
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << std::endl;
    }
    return (0);
}


