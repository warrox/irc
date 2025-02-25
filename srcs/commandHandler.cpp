/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   commandHandler.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cyferrei <cyferrei@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/13 10:48:50 by cyferrei          #+#    #+#             */
/*   Updated: 2025/02/25 09:23:56 by whamdi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Server.hpp"
#include "../includes/colors.hpp"

#include <sstream>
#include <iostream>
#include <string>
#include <unistd.h>


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

void Server::commandHandler(int clientFd, std::string cmd) {
	std::istringstream stream(cmd);
	std::string line;

	while (std::getline(stream, line)) {
		std::istringstream lineStream(line);
		std::string commandName;
		lineStream >> commandName;

		std::map<std::string, CommandFunc>::iterator match = this->_commands.find(commandName);
		if (match != this->_commands.end()) {
			if (line.at(line.size() - 1) == '\r') {
				line.erase(line.end() - 1);
			}
			(this->*_commands[commandName])(clientFd, line);
		} else {
			std::cout << YELLOW << "Unknown or unhandled cmd!" << std::endl;
		}
	}
}
