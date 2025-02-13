/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_user.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cyferrei <cyferrei@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/13 10:40:17 by cyferrei          #+#    #+#             */
/*   Updated: 2025/02/13 11:02:24 by cyferrei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Server.hpp"
#include "../includes/colors.hpp"

#include <sstream>
#include <iostream>

void Server::user(int clientFd, std::string cmd) {

	std::string pass = this->_clients[clientFd].getPassword();	
	if (pass.empty() || this->_pass != pass) {
		std::cout << RED << "Wrong Password connection denied" << RESET << std::endl;
		std::string errorMsg = ":localhost 464 * :Incorrect password\r\n";
		this->sendAndLog(clientFd, errorMsg);

		disconnectClient(clientFd, "Incorrect password");
		return;
	}

	std::istringstream iss(cmd);
	std::string command;
	std::string nick;
	std::string user;
	std::string host;
	std::string realname;

	iss >> command >> nick >> user >> host;
	std::getline(iss, realname);

	if (user.empty() || host.empty() || realname.empty() || realname[1] != ':') {
		sendAndLog(clientFd, ":localhost 461 " + _clients[clientFd].getNick() + " USER :\r\n");
		disconnectClient(clientFd, "Invalid USER command");
		return;
	}

	_clients[clientFd].setUser(user);
	_clients[clientFd].setHost(host);
	_clients[clientFd].setRealName(realname.substr(2));
	
	// reponses IRC pour USER
	
	std::string welcome = ":localhost 001 " + _clients[clientFd].getNick() + " :Welcome to the IRC network, " + _clients[clientFd].getNick() + "!\r\n";
	std::string your_host = ":localhost 002 " + _clients[clientFd].getHost() + " :Your host is " + this->_servername + ", running version 1.0\r\n";
	std::string created = ":localhost 003 " + _clients[clientFd].getNick() + " :This server was created on February\r\n";
	std::string my_info = ":localhost 004 " + _clients[clientFd].getNick() + this->_servername + " 1.0 @cyferrei/@whamdi\r\n";
	std::string full_msg = welcome + your_host + created + my_info;

	sendAndLog(clientFd, full_msg);
}