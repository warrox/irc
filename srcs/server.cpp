#include "../includes/server.hpp"
#include <algorithm>
#include <cstdlib>
#include <exception>
#include <iterator>
#include <sys/poll.h>
#include <sys/socket.h>
#include <cctype>
#include <cstdio>
#include <cstring>
#include <iostream>		
#include <poll.h>
#include <utility>
#include <vector>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdio.h>
#include "../includes/msghandler.hpp"
#include "../includes/colors.hpp"
#include "../includes/Channel.hpp"
#include <sstream>
#include "../includes/Channel.hpp"

server::server(std::string port, std::string password)
{
	this->_port = port;
	this->_pass = password;

	struct sockaddr_in adress; 
	this->_address = adress; 

	socklen_t addrrlen = sizeof(adress);
	this->_addrlen = addrrlen;
    
    this->_commands["NICK"] = &server::nick;
    this->_commands["PASS"] = &server::pass;
	this->_commands["JOIN"] = &server::join;

	//A function should be responsible for an action
	//Orelse it should mention in its name, the actions it'll perform
	//FIX:: 
	bindSocket();
	listenClient();
	acceptClient();
	readInSocket();
}

//All exitfailure in this program should be replaced by a function that
//Close clients sever, destroy the server and leave
void server::fatal(std::string error) {
	std::cerr << __FILE_NAME__ << ":" << __LINE__ << ": Fatal: " << error << std::endl;
	exit(EXIT_FAILURE);
}

void server::bindSocket()
{
    // char buffer[1024] = {0};
    this->_server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (this->_server_fd == 0) 
		this->fatal("Socker creation failed");

    this->_address.sin_family = AF_INET;
    this->_address.sin_addr.s_addr = INADDR_ANY;
	//Using getPort made no sense, pretty sure this variable is useless too
	/*std::string port = this->_port;*/
	try {
		this->_address.sin_port = htons(std::atoi(this->_port.c_str()));
	} catch(const std::exception &e) {
		//The exception should suffice itself, no need to add an explicit message
		//It's supposed to be explicit already
        /*std::cerr << "Invalid port number: " << e.what() << std::endl;*/
		std::cerr << e.what() << std::endl;
		close(this->_server_fd);
		exit(EXIT_FAILURE);
	}

    if (::bind(this->_server_fd, (struct sockaddr*)&this->_address, sizeof(this->_address)) < 0) 
		this->fatal("Port binding failed");
}
void server::listenClient()
{
	if (listen(this->_server_fd, BACKLOG) < 0) 
	{
        perror("listen failed");
        exit(EXIT_FAILURE);
    }
    std::cout << BOLD_ON BLUE << "[serveur]: " << BOLD_OFF BLUE << "server listening on port " << this->_port << BOLD_OFF << std::endl;

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
            this->_clients[new_socket] = Client();
            std::cout << BOLD_ON BLUE << "[serveur]: " << BOLD_OFF BLUE << "new connection accepted!" << BOLD_OFF << std::endl;
            pollfd client_pollfd;
            client_pollfd.fd = new_socket;
            client_pollfd.events = POLLIN;
            this->_pfds.push_back(client_pollfd);
	        this->sendWelcomeMessage(new_socket, "guest");
        }

        for (size_t i = 1; i < this->_pfds.size(); ++i)
        {
            if (this->_pfds[i].revents & POLLIN)
            {
                char message[1024] = {0};
                int valread = read(this->_pfds[i].fd, message, 1024);
                if (valread > 0)
                {
                    message[valread] = '\0';
                    std::cout << BOLD_ON BLUE << "[serveur]" << BOLD_OFF BLUE << "[received]: " << BOLD_OFF << message;

                    std::string buffer = std::string(message);
				    this->commandHandler(this->_pfds[i].fd, buffer);
                }
                else if (valread == 0)
                {
                    std::cout << BOLD_ON BLUE << "[serveur]: " << BOLD_OFF BLUE << "client disconnected" << BOLD_OFF << std::endl;
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
