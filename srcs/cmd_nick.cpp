/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_nick.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cyferrei <cyferrei@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/13 10:41:57 by cyferrei          #+#    #+#             */
/*   Updated: 2025/02/13 10:42:55 by cyferrei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Server.hpp"

#include <sstream>

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
	if (nickname.empty()) {
		sendAndLog(clientFd, ":localhost 461 " + _clients[clientFd].getNick() + " NICK :\r\n");
		return;
	}
	for (std::map<int, Client>::iterator match = _clients.begin(); match != _clients.end(); ++match) {
		if (match->second.getNick() == nickname) {
			sendAndLog(clientFd, ":localhost 433 " + _clients[clientFd].getNick() + " NICK :" + nickname + "\r\n");
			setNewNick(clientFd, nickname);
			return;
		}
	}
	_clients[clientFd].setNick(nickname);
	// ? looking for reponse 
}