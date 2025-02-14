/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   commandHandler.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cyferrei <cyferrei@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/13 10:48:50 by cyferrei          #+#    #+#             */
/*   Updated: 2025/02/14 15:35:02 by whamdi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Server.hpp"
#include "../includes/colors.hpp"

#include <sstream>
#include <iostream>
#include <string>
#include <unistd.h>

//? <prefix>   ::= <servername> | <nick> [ '!' <user> ] [ '@' <host> ]

//? <message>  ::= [':' <prefix> <SPACE> ] <command> <params> <crlf>
//? ex --> his->get_prefix(clientFd) + " " + "MODE " + _clients[clientFd].getUser() + " " + mode + "\r\n");

std::string Server::get_prefix(int clientFd) {
	return (":" + _clients[clientFd].getNick() + "!" + _clients[clientFd].getUser() + "@" + _clients[clientFd].getHost());
}

void Server::disconnectClient(int clientFd, const std::string& reason) {

	if (send(clientFd, "", 0, MSG_NOSIGNAL) != -1) {
		std::string errorMsg = ":localhost ERROR :Closing Link: " + reason + "\r\n";
		sendAndLog(clientFd, errorMsg);
	}

	close(clientFd);
	_clients.erase(clientFd);
	std::cout << RED << "Client " << clientFd << " disconnected: " << reason << RESET << std::endl;
}


void Server::join(int clientFd, std::string cmd) {
	std::istringstream lineStream(cmd);
	std::string cmdName, chanName;
	lineStream >> cmdName;
	lineStream >> chanName;

	std::string server_name = "localhost.irc";
	channelIterator it = this->_channels.find(chanName);

	if (it == this->_channels.end()) {
		channelEntry newEntry(chanName, Channel(chanName, clientFd));
		this->_channels.insert(newEntry);
		this->log("Channel created: " + chanName);

		it = this->_channels.find(chanName);
		it->second.addUser(clientFd);
		this->log("Added client to " + chanName);
		this->sendAndLog(clientFd, ":" + server_name + " JOIN :" + chanName + "\r\n");

	} else {
		it->second.addUser(clientFd);
		this->log("Added client to " + chanName);
		this->sendAndLog(clientFd, ":" + server_name + " JOIN :" + chanName + "\r\n");

		if (!it->second.getTopic().empty()) {
			this->sendAndLog(clientFd, ":" + server_name + " 332 " + _clients[clientFd].getNick() + " " + chanName + " :" + it->second.getTopic() + "\r\n");
		}
	}
}

void Server::commandHandler(int clientFd, std::string cmd) {
	std::istringstream stream(cmd);
	std::string line;

	while (std::getline(stream, line)) {
		std::istringstream lineStream(line);
		std::string commandName;
		lineStream >> commandName;

		std::map<std::string, CommandFunc>::iterator match = this->_commands.find(commandName);
		if (match != this->_commands.end()) {
			line.erase(line.end() - 1);
			(this->*_commands[commandName])(clientFd, line);
		} else {
			std::cout << "Error" << std::endl;
		}
	}
}
