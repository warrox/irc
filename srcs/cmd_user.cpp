/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_user.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cyferrei <cyferrei@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/13 10:40:17 by cyferrei          #+#    #+#             */
/*   Updated: 2025/02/24 19:02:38 by cyferrei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Server.hpp"

#include <sstream>

void Server::user(int clientFd, std::string cmd) {

	std::string pass = this->_clients[clientFd].getPassword();	
	if (pass.empty() || this->_pass != pass) {
		std::string errorMsg = ":" + this->_servername + "464" + " " + _clients[clientFd].getNick() + " Wrong password\r\n";
		this->sendAndLog(clientFd, errorMsg);
		disconnectClient(clientFd, errorMsg);
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
		sendAndLog(clientFd, ":" + this->_servername + "461" + _clients[clientFd].getNick() + " " + " :Missing arguments\r\n");
		return;
	}
	_clients[clientFd].setUser(user);
	_clients[clientFd].setHost(host);
	_clients[clientFd].setRealName(realname.substr(2));
	
	std::string welcome = ":" + this->_servername + " 001 " + _clients[clientFd].getNick() + " :Welcome to the IRC network, " + _clients[clientFd].getNick() + "!" + "\r\n";
	std::string your_host = ":" + this->_servername + " 002 " + _clients[clientFd].getNick() + " :Your host is ft_irc running version 1.0" + "\r\n";
	std::string created = ":" + this->_servername + " 003 " + _clients[clientFd].getNick() + " :This server was created on " + this->_time + "\r\n";
	std::string my_info = ":" + this->_servername + " 004 " + _clients[clientFd].getNick()  + this->_servername + "1.0 @cyferrei/@whamdi" + "\r\n";

	sendAndLog(clientFd, welcome);
	sendAndLog(clientFd, your_host);
	sendAndLog(clientFd, created);
	sendAndLog(clientFd, my_info);

}
