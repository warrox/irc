/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_part.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cyferrei <cyferrei@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/21 12:50:01 by cyferrei          #+#    #+#             */
/*   Updated: 2025/02/24 19:02:13 by cyferrei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Server.hpp"
// #include "../includes/colors.hpp"

// #include <iostream>
#include <map>
#include <sstream>
#include <string>

void Server::part(int clientFd, std::string cmd) {
	
	std::istringstream iss(cmd);
	std::string commamd, channel_name;
	
	iss >> commamd >> channel_name;

	std::map<std::string, Channel>::iterator match = _channels.begin();
	for (; match != _channels.end(); ++match) {
		if (match->first == channel_name)
			break;
	}
	if (match == _channels.end()) {
		sendAndLog(clientFd, this->get_prefix(clientFd) + " 403 " + _clients[clientFd].getNick() + " " + channel_name + " :No such channel\r\n");
		return;
	}
	
	if (_channels[channel_name].isUserInChannel(_clients[clientFd].getNick())) {

		sendAndLog(clientFd, get_prefix(clientFd) + _clients[clientFd].getNick() + " " + "PART " + channel_name + "\r\n");
		_channels[channel_name].broadcast(clientFd, *this, (get_prefix(clientFd) + _clients[clientFd].getNick() + " " + "PART " + channel_name + "\r\n"), true);
		_channels[channel_name].removeNameUserInChannel(_clients[clientFd].getNick());
		_channels[channel_name].removeUserInChannel(_clients[clientFd]);
		_channels[channel_name].delUser(clientFd);
		
		if (_channels[channel_name].getNbUsersInChannel() == 0)
			this->removeChannel(channel_name);
	}
	else 
		this->sendAndLog(clientFd, ":" + this->_clients[clientFd].getNick() + " 441 " + _clients[clientFd].getNick() + " " + channel_name + " :Not in channel\r\n");
}
