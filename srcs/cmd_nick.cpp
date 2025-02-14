/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_nick.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cyferrei <cyferrei@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/13 10:41:57 by cyferrei          #+#    #+#             */
/*   Updated: 2025/02/14 11:20:16 by cyferrei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Server.hpp"
#include "../includes/colors.hpp"

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
	std::cout << YELLOW << "|" << command << "|" << std::endl;
	std::cout << YELLOW << "|" << nickname << "|" << std::endl;
	if (nickname.empty()) {
		sendAndLog(clientFd, ":localhost 461 " + _clients[clientFd].getNick() + " NICK :\r\n");
		return;
	}
	for (std::map<int, Client>::iterator match = _clients.begin(); match != _clients.end(); ++match) {
		if (match->second.getNick() == nickname) {
			setNewNick(clientFd, nickname);
			sendAndLog(clientFd, this->get_prefix(clientFd) + " " + "NICK " + _clients[clientFd].getUser() + " " + _clients[clientFd].getNick()+ "\r\n");
			return;
		}
	}
	_clients[clientFd].setNick(nickname);
	sendAndLog(clientFd, this->get_prefix(clientFd) + " " + "NICK " + _clients[clientFd].getUser() + " " + _clients[clientFd].getNick() + "\r\n");
}