/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   commandHandler.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cyferrei <cyferrei@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/13 10:48:50 by cyferrei          #+#    #+#             */
/*   Updated: 2025/02/13 11:36:59 by cyferrei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Server.hpp"
#include "../includes/colors.hpp"

#include <sstream>
#include <iostream>
#include <string>
#include <unistd.h>

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

	//Declare two strings at once
	std::string cmdName, chanName;
	lineStream >> cmdName;
	lineStream >> chanName;

	//We made a typedef together for readability, use it
    channelIterator it = this->_channels.find(chanName);

	if(it == this->_channels.end()) {
		//Added a new typdef, which is a pair of std::string, Channel::Channel
		//We call every pair < key, value > in a map an Entry, see map definition on cpp reference
		//Improved readability.
		channelEntry newEntry(chanName, Channel(chanName, clientFd));
		this->_channels.insert(newEntry);
		this->log("Channel created: " + chanName);
		it = this->_channels.find(chanName);
		it->second.addUser(clientFd);
		std::string log = "Added client to " + chanName;
		this->log(log);
		this->sendAndLog(clientFd, ":username!ident@hostname JOIN : #" + chanName);

	} else {
		//Add the client before logging the information, doesn't make sense othewise
		//If you have an exception in addUser, you'll never get where it came from cuz you
		//Asserted it was ok by logging before actually updating the value
		it->second.addUser(clientFd);
		std::string log = "Added client to " + chanName;	
		this->sendAndLog(clientFd, ":username!ident@hostname JOIN : #" + chanName);

		if(it->second.getTopic() != "")
		{
			this->sendAndLog(clientFd, ":irc.example.com 332 username #channel : " + it->second.getTopic());
		}
		this->log(log);
		/*it->second.addUser(clientFd);*/
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
			(this->*_commands[commandName])(clientFd, line);
		} else {
			std::stringstream log;
			log << commandName << ": Command not recognized";
			this->log(log.str());
		}
	}
}
