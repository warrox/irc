/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_join.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cyferrei <cyferrei@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/13 14:13:09 by whamdi            #+#    #+#             */
/*   Updated: 2025/02/25 16:08:54 by cyferrei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Server.hpp"
#include "../includes/Channel.hpp"

#include <sstream> 
#include <iostream>
#include <vector>

void Server::sendingUserListToClient(std::string chanName, int clientFd, bool is_first_client)
{
	(void) is_first_client;

	std::string userList = ":" + _servername + " 353 " + this->_clients[clientFd].getNick() + " = " + chanName + " :";
	std::string endOfUserList = ":" + _servername + " 366 " + this->_clients[clientFd].getNick() + " " + chanName + " :End of member list\r\n";

	const std::vector<Client>& users = this->_channels[chanName].getUsers();
	for (std::vector<Client>::const_iterator it = users.begin(); it != users.end(); ++it) {
		if (it->getModeO()) 
		{
			userList += " @" + it->getNick();
		} else {
			userList += " " + it->getNick();
		}
	}

	userList += "\r\n";
	this->sendAndLog(clientFd, userList);
	this->sendAndLog(clientFd, endOfUserList);
}

void Server::join(int clientFd, std::string cmd) 
{
	std::istringstream lineStream(cmd);
	std::string cmdName, chanName, args;
	lineStream >> cmdName >> chanName >> args;

	channelIterator it = this->_channels.find(chanName);
	if (it == this->_channels.end()) 
	{
		this->_channels.insert(channelEntry(chanName, Channel(chanName, *this)));
		it = this->_channels.find(chanName);

		this->_clients[clientFd].setModeO(true);
		it->second.addUser(clientFd);
		it->second.addUserInChannel(_clients[clientFd]);
		it->second.addNameInListChannel(_clients[clientFd].getNick());

		std::string joinMsg = this->get_prefix(clientFd) + " JOIN :" + chanName + "\r\n";
		this->sendAndLog(clientFd, joinMsg);
		sendingUserListToClient(chanName, clientFd, true);
	}
	else 
	{
		if (_channels[chanName].getModeK() && args != _channels[chanName].getKeyChannel()) {
			sendAndLog(clientFd, get_prefix(clientFd) + " 475 " + _clients[clientFd].getNick() + " " + chanName + " :Wrong channel key\r\n");
			return;
		}
		if (_channels[chanName].getModeL() && ((int)_channels[chanName].getNbUsersInChannel() >= _channels[chanName].getLimitValue())) {
			sendAndLog(clientFd, ":" + _servername + " 471 " + _clients[clientFd].getNick() + " " + chanName + " :Channel is now full\r\n");
			return;
		}
		if (_channels[chanName].getModeI() && !(_channels[chanName].isUserInvitedInChannel(_clients[clientFd].getNick()))) {
			sendAndLog(clientFd, ":" + _servername + " 473 " + _clients[clientFd].getNick() + " " + chanName + " :You must be invited to join this channel\r\n");
			return;
		}

		it->second.addUser(clientFd);
		it->second.addUserInChannel(_clients[clientFd]);
		it->second.addNameInListChannel(_clients[clientFd].getNick());

		std::string joinMsg = this->get_prefix(clientFd) + " JOIN :" + chanName + "\r\n";
		this->sendAndLog(clientFd, joinMsg);

		if (!it->second.getTopic().empty()) {
			this->sendAndLog(clientFd, ":" + this->_clients[clientFd].getNick() + " 332 " + _clients[clientFd].getNick() + " " + chanName + " :" + it->second.getTopic() + "\r\n");
		}
		
		it->second.broadcast(clientFd, *this, joinMsg, true);
		sendingUserListToClient(chanName, clientFd, false);
	}
}

