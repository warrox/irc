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
#define BACKLOG 5
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
	bindSocket();
	listenClient();
	acceptClient();
	readInSocket();

	// sendMessage("Well done");

}
server::~server(){}
void server::join(int fd, std::string cmd)
{
	std::istringstream lineStream(cmd);
	std::string cmdName; 
	std::string chanName;
	lineStream >> cmdName;
	lineStream >> chanName;

	// channelIterator it = std::find(this->_channels.begin(), this->_channels.end(),chanName);	
    std::map<std::string, Channel>::iterator it = this->_channels.find(chanName);
	if(it == this->_channels.end())
	{
		this->_channels.insert(std::make_pair(chanName, Channel(chanName, fd)));	
	}
	else
	{
		std::cout << "Added client to  " << chanName <<std::endl;
		it->second.addUser(fd);
	}
	
}
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
