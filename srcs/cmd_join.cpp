/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_join.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: whamdi <whamdi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/13 14:13:09 by whamdi            #+#    #+#             */
/*   Updated: 2025/02/13 14:24:08 by whamdi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Server.hpp"
#include <sstream> 

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

