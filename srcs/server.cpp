#include "../includes/lib.hpp"
#include <cstdlib>
#include <exception>
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
	this->_new_socket = accept(this->_server_fd, (struct sockaddr*)&this->_address, &this->_addrlen);
    if (this->_new_socket < 0) {
        perror("accept failed");
        exit(EXIT_FAILURE);
    }
    std::cout << "Connection accepted!" << std::endl;
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
