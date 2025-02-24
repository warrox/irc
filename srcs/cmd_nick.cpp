/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_nick.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cyferrei <cyferrei@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/13 10:41:57 by cyferrei          #+#    #+#             */
/*   Updated: 2025/02/24 15:36:15 by cyferrei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Server.hpp"

#include <sstream>
#include <iostream>

bool Server::isNickTaken(std::string nickname){

	for (std::map<int, Client>::const_iterator it = _clients.begin(); it != _clients.end(); ++it) {
		if (it->second.getNick() == nickname)
			return true;
	}
	return false;
}

void Server::setNewNick(int clienFd, std::string nickname) {
	static int suf = 0;
	std::stringstream new_nickname;

	while (isNickTaken(new_nickname.str())) {
		new_nickname << nickname << suf;
		suf++;
	}
	_clients[clienFd].setNick(new_nickname.str());
}

void Server::nick(int clientFd, std::string cmd) {

	std::istringstream iss(cmd);
	std::string command;
	std::string nickname;

	iss >> command >> nickname;
	if (_clients[clientFd].getPassword().empty() || _clients[clientFd].getPassword() != this->_pass) {
		std::string errorMsg = ":" + this->_servername + "464" + " " + _clients[clientFd].getNick() + " :Wrong password\r\n";
		this->sendAndLog(clientFd, errorMsg);

		disconnectClient(clientFd, "Incorrect password");
		return;
	}
	if (nickname.empty()) {
		sendAndLog(clientFd, ":" + this->_servername + "431" + " " + _clients[clientFd].getNick() + " " + nickname + " :No nickname given\r\n");
		return;
	}
	for (std::map<int, Client>::iterator match = _clients.begin(); match != _clients.end(); ++match) {
		if (match->second.getNick() == nickname) {
			sendAndLog(clientFd, ":" + this->_servername + "433" + " " + _clients[clientFd].getNick() + " " + nickname + " :Nickname in use\r\n");
			if (_clients[clientFd].getNick().empty()) {
				setNewNick(clientFd, "guest");
				sendAndLog(clientFd, this->get_prefix(clientFd) + " " + "NICK :" + _clients[clientFd].getNick()+ "\r\n");
			}
			return;
		}
	}
	std::string old_nick = _clients[clientFd].getNick();
	_clients[clientFd].setNick(nickname);
	std::string response = ":" + old_nick + "!" + _clients[clientFd].getUser() + "@" + _clients[clientFd].getHost() + " NICK :" + nickname + "\r\n";
	sendAndLog(clientFd, response);

	std::string is_in_channel = isClientInAChannel(_clients[clientFd].getNick());
	if (is_in_channel.empty())
		return;
	else {
		std::cout << "ICI" << std::endl;
		// this->sendNickChange(clientFd, old_nick, nickname);
		_channels[is_in_channel].broadcast(clientFd, *this, response, false, true);
	}
}
