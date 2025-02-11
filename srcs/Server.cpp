#include "../includes/Server.hpp"
#include "../includes/colors.hpp"

#include <cstdlib>
#include <exception>
#include <sstream>
#include <sys/poll.h>
#include <sys/socket.h>
#include <cctype>
#include <cstdio>
#include <cstring>
#include <iostream>		
#include <poll.h>
#include <vector>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdio.h>

Server::Server(std::string port, std::string password)
{
	this->_port = port;
	this->_pass = password;

	struct sockaddr_in adress; 
	this->_address = adress; 

	socklen_t addrrlen = sizeof(adress);
	this->_addrlen = addrrlen;
    
    this->_commands["NICK"] = &Server::nick;
    this->_commands["PASS"] = &Server::pass;
	this->_commands["JOIN"] = &Server::join;

	//A function should be responsible for an action
	//Orelse it should mention in its name, the actions it'll perform
	//FIX:: 
	/*bindSocket();*/
	/*listenClient();*/
	/*acceptClient();*/
	/*readInSocket();*/
}

void Server::start()
{
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

	if (listen(this->_server_fd, BACKLOG) < 0) 
		this->fatal("Listen failed");

	std::string log = "Server started listening on port " + this->_port;
	this->log(log);
}

void Server::acceptNewClient() {
	int new_socket = accept(this->_server_fd, (struct sockaddr *)&this->_address, &this->_addrlen);
	if (new_socket < 0)
		this->fatal("Accept failed");

	this->_clients[new_socket] = Client();

	std::stringstream log;
	log << "New connection accepted on fd: " << new_socket;
	this->log(log.str());

	pollfd client_pollfd;
	client_pollfd.fd = new_socket;
	client_pollfd.events = POLLIN;
	this->_pfds.push_back(client_pollfd);

	this->sendWelcomeMessage(new_socket, "guest");
}

void Server::scanClients() {
	for (size_t i = 1; i < this->_pfds.size(); ++i) {
		//Inverted the condition to avoid extra indentation level + its faster
		if ((this->_pfds[i].revents & POLLIN) == 0)
			continue;
		/*if (this->_pfds[i].revents & POLLIN)*/
		/*{*/
		//1024 ????? #define
		/*char message[1024] = {0};*/
		char message[BUFFER_SIZE] = {0};
		int valread = read(this->_pfds[i].fd, message, BUFFER_SIZE);
		if (valread < 0)
			this->fatal("Read failed");

		if (!valread) {
			std::stringstream log;
			log << "Client disconnected: " << this->_pfds[i].fd;
			this->log(log.str());
			close(this->_pfds[i].fd);
			this->_pfds.erase(this->_pfds.begin() + i);
			--i; //Dangerous
		} else {
			message[valread] = '\0';
			this->recvLog(this->_pfds[i].fd, message);
			this->commandHandler(this->_pfds[i].fd, std::string(message));
		}
	}
}

void Server::run()
{
    pollfd server_pollfd;
    server_pollfd.fd = this->_server_fd;
    server_pollfd.events = POLLIN;
    this->_pfds.push_back(server_pollfd);

    while (true)
    {
        int activity = poll(this->_pfds.data(), this->_pfds.size(), -1);
        if (activity < 0)
			this->fatal("Poll failed");

        if (this->_pfds[0].revents & POLLIN)
			this->acceptNewClient();

		//Check for every clients if a packet has been sent
		this->scanClients();

    }
}
