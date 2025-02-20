/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_join.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cyferrei <cyferrei@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/13 14:13:09 by whamdi            #+#    #+#             */
/*   Updated: 2025/02/20 15:45:06 by cyferrei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Server.hpp"
#include "../includes/Channel.hpp"

#include <sstream> 
#include <iostream>

void Server::sendingUserListToClient(std::string chanName, int clientFd, bool is_first_client)
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
    }

    userList += "\r\n";
    this->sendAndLog(clientFd, userList);
    this->sendAndLog(clientFd, endOfUserList);
}

void Server::join(int clientFd, std::string cmd) 
{
	std::istringstream lineStream(cmd);
	std::string cmdName, chanName, args;
	lineStream >> cmdName;
	lineStream >> chanName;
	lineStream >> args;
	
	channelIterator it = this->_channels.find(chanName);
	if (it == this->_channels.end()) 
	{
		std::cout << "FIRST call" << std::endl;
		channelEntry newEntry(chanName, Channel(chanName,*this));
		this->_channels.insert(newEntry);
		// this->log("Channel created: " + chanName);

		it = this->_channels.find(chanName);
		this->_clients[clientFd].setModeO(true);
		it->second.addUser(clientFd);
		//TEST
		it->second.addUserInChannel(_clients[clientFd]);
		// this->log("Added client to " + chanName);
		this->sendAndLog(clientFd, ":" + this->_clients[clientFd].getNick() + " JOIN :" + chanName + "\r\n");
		sendingUserListToClient(chanName, clientFd, true);
    }
	else 
	{
		//**************************************************MODE K*************************************************** */
		if (_channels[chanName].getModeK() && args == _channels[chanName].getKeyChannel()) {
			if (_channels[chanName].getModeL()) {
				if ((int)_channels[chanName].getNbUsersInChannel() < _channels[chanName].getLimitValue()) {
					it->second.addUser(clientFd);
					it->second.addUserInChannel(_clients[clientFd]);
					this->sendAndLog(clientFd, ":" + this->_clients[clientFd].getNick() + " JOIN :" + chanName + "\r\n");
					return;
				}
				else {
					sendAndLog(clientFd, ":" + _servername + " 471 " + _clients[clientFd].getNick() + " " + chanName + " :Channel is now full" + "\r\n");
					return;
				}
			}
			else {
				it->second.addUser(clientFd);
				it->second.addUserInChannel(_clients[clientFd]);
				this->sendAndLog(clientFd, ":" + this->_clients[clientFd].getNick() + " JOIN :" + chanName + "\r\n");
				return;
			}
		}
		else if (_channels[chanName].getModeK() && args != _channels[chanName].getKeyChannel()) {
			sendAndLog(clientFd, get_prefix(clientFd) + " 475 " + _clients[clientFd].getNick() + " " + chanName + " :Wrong channel key" + "\r\n");
			return;
		}
		//**************************************************MODE L*************************************************** */
		if (_channels[chanName].getModeL() && ((int)_channels[chanName].getNbUsersInChannel() < _channels[chanName].getLimitValue())) {
			if (_channels[chanName].getModeK()) {
				if (_channels[chanName].getKeyChannel() == args) {
					it->second.addUser(clientFd);
					it->second.addUserInChannel(_clients[clientFd]);
					this->sendAndLog(clientFd, ":" + this->_clients[clientFd].getNick() + " JOIN :" + chanName + "\r\n");
					return;
				}
				else {
					sendAndLog(clientFd, get_prefix(clientFd) + " 475 " + _clients[clientFd].getNick() + " " + chanName + " :Wrong channel key" + "\r\n");
					return;
				}
			}
			else {
				it->second.addUser(clientFd);
				it->second.addUserInChannel(_clients[clientFd]);
				this->sendAndLog(clientFd, ":" + this->_clients[clientFd].getNick() + " JOIN :" + chanName + "\r\n");
				return;
			}
		}
		else if (_channels[chanName].getModeL() && ((int)_channels[chanName].getNbUsersInChannel() == _channels[chanName].getLimitValue())) {
			sendAndLog(clientFd, ":" + _servername + " 471 " + _clients[clientFd].getNick() + " " + chanName + " :Channel is now full" + "\r\n");
			return;
		}
		if (!it->second.getTopic().empty()) 
			this->sendAndLog(clientFd, ":" + this->_clients[clientFd].getNick() + " 332 " + _clients[clientFd].getNick() + " " + chanName + " :" + it->second.getTopic() + "\r\n");
		sendingUserListToClient(chanName, clientFd, false);
	}
}