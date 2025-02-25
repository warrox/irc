/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cyferrei <cyferrei@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/13 10:48:24 by cyferrei          #+#    #+#             */
/*   Updated: 2025/02/25 15:03:51 by whamdi           ###   ########.fr       */
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
#include <cstdlib>

std::string Server::isClientInAChannel(std::string name) {
	
	std::map<std::string, Channel>::iterator match = _channels.begin();
	for (; match != _channels.end(); ++match) {
		if (match->second.isUserInChannel(name))
			return (match->first);
	}
	return "";
}

std::map<int,Client> Server::getClients()const
{
	return(this->_clients);
}
void setNonBlocking(int socket) {
    if (fcntl(socket, F_SETFL, O_NONBLOCK) == -1) {
        perror("fcntl F_SETFL");
        exit(EXIT_FAILURE);
    }
}
void Server::shutDown(void) {
	std::map<int, Client>::iterator it = this->_clients.begin();
	for (; it != this->_clients.end(); ++it) {
		close(it->first);
	}
	close(this->_socket);
}

void Server::sendMessageto(int clientFd, int receiver_fd, std::string msg, std::string sender_name, std::string receiver_name)
{
	(void)sender_name;
	
	std::string formatedMsg = this->get_prefix(clientFd) + " PRIVMSG " + receiver_name + " :" + msg + "\r\n";
	sendAndLog(receiver_fd, formatedMsg);
}

Server::Server(std::string port, std::string password) {
	this->_port = port;
	this->_pass = password;
	this->_servername = "ft_irc";
	this->_time = "25-02-2025 12:00";

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
	this->_commands["PART"] = &Server::part;

}

void Server::start() {
	struct sockaddr_in server_address = {};
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
	this->_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (this->_socket == -1) {
		this->fatal("Failed to create socket");
		return;
	}

	// this is just a convenience to always have the same port and not have to wait for the kernel to release it.
	if (setsockopt(this->_socket, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &optname, socklen) == -1) {
		close(this->_socket);
		this->fatal("Failed to configure socket to SO_REUSEADDR | SO_REUSEPORT");
		return;
	}

	// you need to make sure client's are also configured in non-blocking mode.
	if (fcntl(this->_socket, F_SETFL, O_NONBLOCK) == -1) {
		close(this->_socket);
		this->fatal("Failed to configure socket to O_NONBLOCK");
		return;
	}

	if (bind(this->_socket, (struct sockaddr *)&server_address, socklen) == -1) {
		close(this->_socket);
		this->fatal("Failed to bind socket.");
		return;
	}

	if (listen(this->_socket, SOMAXCONN) == -1) {
		close(this->_socket);
		this->fatal("Failed to listen with socket.");
		return;
	}

	this->_address   = server_address;
	this->_server_fd = this->_socket;
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
	static std::string input;
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
				break;
			} else if(rbytes > 0) {
				buffer[rbytes] = '\0';
				input.append(buffer, rbytes);
				size_t pos;
				// this->recvLog(client.fd, input);
				while ((pos = input.find('\n')) != std::string::npos) {
					std::string command = input.substr(0, pos);
					input.erase(0, pos + 1);
					this->commandHandler(client.fd, command);
				}
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

void Server::run(){
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


void Server::removeChannel(std::string channel_name) {

	std::map<std::string, Channel>::iterator match = _channels.find(channel_name);
	if (match != _channels.end()) 
		_channels.erase(match);
}
