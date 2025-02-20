/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cyferrei <cyferrei@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/13 10:48:24 by cyferrei          #+#    #+#             */
/*   Updated: 2025/02/20 10:42:56 by whamdi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Server.hpp"

#include <exception>
#include <sys/poll.h>
#include <sys/socket.h>
#include <cstdio>
#include <vector>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>
#include <ctime>
#include <iostream>
#include <cstdlib>

std::string getTime() {
	std::time_t now = std::time(0);
	std::tm* now_tm = std::localtime(&now);

	char buffer[20];
	std::strftime(buffer, sizeof(buffer), "%d-%m-%Y %H:%M:%S", now_tm);

	return std::string(buffer);
}

std::map<int,Client> Server::getClients()const
{
	return(this->_clients);
}
void Server::sendMessageto(int clientFd, int receiver_fd, std::string msg, std::string sender_name, std::string receiver_name)
{
	std::cout << "Message before sending to client : " << msg << std::endl;
	// std::string formatedMsg = ":" + sender_name + " PRIVMSG " + receiver_name + " :" + msg + "\r\n";
	(void)sender_name;	
	
	std::string formatedMsg = this->get_prefix(clientFd) + " PRIVMSG " + receiver_name + " :" + msg + "\r\n";
	std::cout << "Message size: " << formatedMsg.size() << std::endl;
    std::cout << "Message to send: '" << formatedMsg << "'" << std::endl;
	sendAndLog(receiver_fd, formatedMsg);
}

Server::Server(std::string port, std::string password) {
	this->_port = port;
	this->_pass = password;
	this->_servername = "ft_irc";
	this->_time = getTime();

	struct sockaddr_in adress;
	this->_address	   = adress;

	socklen_t addrrlen = sizeof(adress);
	this->_addrlen = addrrlen;

	this->_commands["PASS"] = &Server::pass;
	this->_commands["NICK"] = &Server::nick;
	this->_commands["USER"] = &Server::user;
	this->_commands["MODE"] = &Server::mode;
	this->_commands["JOIN"] = &Server::join;
	this->_commands["TOPIC"] = &Server::topic;
	this->_commands["PRIVMSG"] = &Server::privmsg;
	this->_commands["QUIT"] = &Server::quit;
	this->_commands["PING"] = &Server::ping;
	this->_commands["KICK"] = &Server::kick;
	this->_commands["INVITE"] = &Server::invite;

}

void Server::start() {
	struct sockaddr_in server_address = {};
	int32_t server_socket = -1;
	const socklen_t socklen	= sizeof(struct sockaddr_in);
	int32_t optname	= 1;

	try {
		server_address.sin_family = AF_INET;
		server_address.sin_addr.s_addr = INADDR_ANY;
		server_address.sin_port	= htons(std::atoi(this->_port.c_str()));

	} catch (const std::exception &e) {
		this->fatal(e.what());
		return;
	}

	// here IPPROTO_TCP is what you will get anyway but it's explicit.
	server_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (server_socket == -1) {
		this->fatal("Failed to create socket");
		return;
	}

	// this is just a convenience to always have the same port and not have to wait for the kernel to release it.
	if (setsockopt(server_socket, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &optname, socklen) == -1) {
		close(server_socket);
		this->fatal("Failed to configure socket to SO_REUSEADDR | SO_REUSEPORT");
		return;
	}

	// you need to make sure client's are also configured in non-blocking mode.
	if (fcntl(server_socket, F_SETFL, O_NONBLOCK) == -1) {
		close(server_socket);
		this->fatal("Failed to configure socket to O_NONBLOCK");
		return;
	}

	if (bind(server_socket, (struct sockaddr *)&server_address, socklen) == -1) {
		close(server_socket);
		this->fatal("Failed to bind socket.");
		return;
	}

	if (listen(server_socket, SOMAXCONN) == -1) {
		close(server_socket);
		this->fatal("Failed to listen with socket.");
		return;
	}

	this->_address   = server_address;
	this->_server_fd = server_socket;
	std::string log = "Server started listening on port " + this->_port;
	this->log(log);
}


void Server::acceptNewClient() {
	struct sockaddr client_address = {};
	int32_t client_socket  = -1;
	socklen_t socklen = sizeof(struct sockaddr_in);

	client_socket = accept(_server_fd, &client_address, &socklen);
	if (client_socket == -1) {
		this->fatal("Failed to accept new client.");
	}

	this->_clients[client_socket] = Client();

	// Ajout du client à la liste des pollfd
	pollfd client_pollfd;
	client_pollfd.fd = client_socket;
	client_pollfd.events = POLLIN;
	this->_pfds.push_back(client_pollfd);

}


void Server::scanClients() {
	char buffer[BUFFER_SIZE] = {0};
	for (size_t i = 1; i < this->_pfds.size(); ++i) {
		struct pollfd &client = _pfds[i];

		if (client.revents == 0) { // no events so we can skip
			continue;
		}

		// those flags are set up automatically by poll to indicate that there was an error.
		// or that the client disconnected.
		if ((client.revents & POLLERR) == POLLERR or (client.revents & POLLHUP) == POLLHUP) {
			// insert logic to remove client.
			close(client.fd);
			_pfds.erase(_pfds.begin() + i);
			// disconnectClient(client.fd, "Read error");
		}

		// this is the flag that indicates that the client is ready to be read  by our server.
		if (client.revents & POLLIN) {
			const int32_t buffsize = sizeof(buffer) / sizeof(buffer[0]);
			int32_t rbytes = recv(client.fd, buffer, buffsize, 0);

			if (rbytes == -1) {
				this->fatal("Failed to read client's request.");
				close(client.fd);
				_pfds.erase(_pfds.begin() + i);
				break;
			} else if (rbytes == 0) {
				close(client.fd);
				_pfds.erase(_pfds.begin() + i);		
				// disconnectClient(client.fd, "Read error");
				break;
			} else {
				buffer[rbytes] = '\0';
				this->recvLog(client.fd, buffer);
				this->commandHandler(client.fd, std::string(buffer));
			}

			client.revents = POLLOUT; // always change POLLIN --> POLLOUT.
			continue;
		}

		// this is the flag that indicates that the client is ready to be written to by our server.
		// if you don't do the switch poll will never wait and harrass you with notification.
		if (client.revents & POLLOUT) {
			// handle sending data to client.

			int32_t wbytes = send(client.fd, "Hi", 2, 0);
			if (wbytes == -1) {
				this->fatal("Failed to read client's request.");
				close(client.fd);
				_pfds.erase(_pfds.begin() + i);
				break;
			}

			client.revents = POLLIN; // always change POLLOUT --> POLLIN.
			continue;
		}
	}
}

void Server::run() {
	pollfd server_pollfd;
	server_pollfd.fd = this->_server_fd;
	server_pollfd.events = POLLIN;
	this->_pfds.push_back(server_pollfd);

	while (true) {
		int activity = poll(this->_pfds.data(), this->_pfds.size(), -1);
		if (activity < 0) this->fatal("Poll failed");

		if (this->_pfds[0].revents & POLLIN) {
			this->acceptNewClient();
		} else {
			this->scanClients();
		}
	}
}
