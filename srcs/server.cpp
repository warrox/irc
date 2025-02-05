#include "../includes/lib.hpp"
#include <cstdlib>
#include <exception>
#include <sys/poll.h>
#include <sys/socket.h>
#include <cctype>
#include <cstdio>
#include <cstring>
#include <iostream>
#include <sstream>
#include <poll.h>
#include <vector>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdio.h>
#include <functional>

/* 	public :
		server(std::string port, std::string password);
		~server();
		//todo
		std::string getPort(void);
		std::string getPass(void);
		void listen();
		void bind();
		void accept();
		void read();
		void send();
*/
// #define PORT 8080
#define BACKLOG 5
server::server(std::string port, std::string password)
{
	this->_port = port;
	this->_pass = password;
	struct sockaddr_in adress; 
	this->_address = adress; 
	socklen_t addrrlen = sizeof(adress);
	this->_addrlen = addrrlen;
	bindSocket();
	listenClient();
	acceptClient();
	readInSocket();
	sendMessage("Well done");

}
server::~server(){}
std::string server::getPort(void)
{
	return(this->_port);
}
void server::bindSocket()
{
    // char buffer[1024] = {0};
    this->_server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (this->_server_fd == 0) 
	{
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    this->_address.sin_family = AF_INET;
    this->_address.sin_addr.s_addr = INADDR_ANY;
	std::string port = this->getPort();	
	try
	{
		this->_address.sin_port = htons(std::atoi(port.c_str()));
	}
	catch(const std::exception &e)
	{
        std::cerr << "Invalid port number: " << e.what() << std::endl;
		close(this->_server_fd);
		exit(EXIT_FAILURE);
	}

    if (::bind(this->_server_fd, (struct sockaddr*)&this->_address, sizeof(this->_address)) < 0) 
	{
        perror("bind failed");
        exit(EXIT_FAILURE);
    }

}
void server::listenClient()
{
	if (listen(this->_server_fd, BACKLOG) < 0) 
	{
        perror("listen failed");
        exit(EXIT_FAILURE);
    }
    std::cout << "Server listening on port " << this->_port << std::endl;

}

void server::acceptClient()
{
    pollfd server_pollfd;
    server_pollfd.fd = this->_server_fd;
    server_pollfd.events = POLLIN;
    this->_pfds.push_back(server_pollfd);

    while (true)
    {
        int activity = poll(this->_pfds.data(), this->_pfds.size(), -1);
        if (activity < 0)
        {
            perror("poll failed");
            continue;
        }

        if (this->_pfds[0].revents & POLLIN)
        {
            int new_socket = accept(this->_server_fd, (struct sockaddr *)&this->_address, &this->_addrlen);
            if (new_socket < 0)
            {
                perror("accept failed");
                exit(EXIT_FAILURE);
            }
            std::cout << "New connection accepted!" << std::endl;

            pollfd client_pollfd;
            client_pollfd.fd = new_socket;
            client_pollfd.events = POLLIN;
            this->_pfds.push_back(client_pollfd);
        }

        for (size_t i = 1; i < this->_pfds.size(); ++i)
        {
            if (this->_pfds[i].revents & POLLIN)
            {
                char buffer[1024] = {0};
                int valread = read(this->_pfds[i].fd, buffer, 1024);
                if (valread > 0)
                {
                    buffer[valread] = '\0';
                    std::cout << "Received: " << buffer << std::endl;
                    send(this->_pfds[i].fd, "Well done", 9, 0);
                }
                else if (valread == 0)
                {
                    std::cout << "Client disconnected" << std::endl;
                    close(this->_pfds[i].fd);
                    this->_pfds.erase(this->_pfds.begin() + i);
                    --i;
                }
                else
                {
                    perror("read failed");
                }
            }
        }
    }
}

void server::readInSocket()
{
	read(this->_new_socket, this->_buffer, 1024);
    std::cout << "Received: " << this->_buffer << std::endl;
	
}
void server::sendMessage(std::string message)
{
	send(this->_new_socket, message.c_str(), message.length(), 0);
    close(this->_new_socket);
    close(this->_server_fd);

}
