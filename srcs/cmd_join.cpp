/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_join.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: whamdi <whamdi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/13 14:13:09 by whamdi            #+#    #+#             */
/*   Updated: 2025/02/18 10:45:07 by whamdi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Server.hpp"
#include <sstream> 
#include <iostream>

void Server::sendingUserListToClient(std::string chanName,int clientFd, bool is_first_client)
{
	(void) is_first_client;

	std::string userList = ":" + _servername + " 353 " + this->_clients[clientFd].getNick() + " = " + chanName + " :";
	std::string endOfUserList = ":" + _servername + " 366 " + this->_clients[clientFd].getNick() + " " + chanName + " :End of member list\r\n";
	const std::vector<Client>& users = this->_channels[chanName].getUsers();
	for (std::vector<Client>::const_iterator it = users.begin(); it != users.end(); ++it) {
		if (it->getModeO()) 
		{
			userList += "@" + it->getNick();
		} else {
			userList += " " + it->getNick();
		}
		userList += "\r\n";
		this->log("Sending the userList to client : " + userList);
		this->sendAndLog(clientFd,userList);
		this->sendAndLog(clientFd, endOfUserList);
	}
}

void Server::join(int clientFd, std::string cmd) 
{
	std::istringstream lineStream(cmd);
	std::string cmdName, chanName;
	lineStream >> cmdName;
	lineStream >> chanName;

	channelIterator it = this->_channels.find(chanName);

	if (it == this->_channels.end()) 
	{
		std::cout << "FIRST call" << std::endl;
		channelEntry newEntry(chanName, Channel(chanName, clientFd,*this));
		this->_channels.insert(newEntry);
		this->log("Channel created: " + chanName);

		it = this->_channels.find(chanName);
		this->_clients[clientFd].setModeO(true);
		it->second.addUser(clientFd);
		this->log("Added client to " + chanName);
		this->sendAndLog(clientFd, ":" + this->_clients[clientFd].getNick() + " JOIN :" + chanName + "\r\n");
		sendingUserListToClient(chanName, clientFd, true);
    }


	else 
	{
		it->second.addUser(clientFd);
		this->log("Added client to " + chanName);
		this->sendAndLog(clientFd, ":" + this->_clients[clientFd].getNick() + " JOIN :" + chanName + "\r\n");

		if (!it->second.getTopic().empty()) 
			this->sendAndLog(clientFd, ":" + this->_clients[clientFd].getNick() + " 332 " + _clients[clientFd].getNick() + " " + chanName + " :" + it->second.getTopic() + "\r\n");
		sendingUserListToClient(chanName, clientFd, false);
	}
}

