/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_mode.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cyferrei <cyferrei@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/13 10:38:25 by cyferrei          #+#    #+#             */
/*   Updated: 2025/02/20 17:30:59 by cyferrei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Server.hpp"
#include "../includes/colors.hpp"

#include <cstddef>
#include <map>
#include <sstream>
#include <iostream>
#include <string>
#include <sys/socket.h>

void Server::mode_k(int clientFd, bool addMode, std::string target, std::string target_user, std::string mode, std::map<std::string, Channel>::iterator match) {
	
	if (!_clients[clientFd].getModeO()) {
		sendAndLog(clientFd, this->get_prefix(clientFd) + " 482 " + _clients[clientFd].getNick() + " " + target + " :Not enought privileges" + "\r\n");
		return;
	}
	
	if (addMode && target_user.empty()) {
		sendAndLog(clientFd, get_prefix(clientFd) + " 461 " + _clients[clientFd].getNick() + " " + target + " " + mode + " :Need more argument" + "\r\n");
		return;
	}
	
	if (!addMode) {
		match->second.setModeK(addMode);
		match->second.setKeyChannel("");
		
		std::string tmp = ":" + _servername + " MODE " + target + " " + mode + "\r\n";
		sendAndLog(clientFd, tmp);
		match->second.broadcast(clientFd, *this, tmp, true);
		return;
	}
	if (addMode && target_user != "") {
		match->second.setModeK(addMode);
		match->second.setKeyChannel(target_user);

		std::string tmp = ":" + _servername + " MODE " + target + " " + mode + " " + target_user + "\r\n";
		sendAndLog(clientFd, tmp);
		match->second.broadcast(clientFd, *this, tmp, true);
		return;
	}
	else {
		sendAndLog(clientFd, get_prefix(clientFd) + " 461 " + _clients[clientFd].getNick() + " " + target + " " + mode + " :Invalid key" + "\r\n");
		return;
	}
}


void Server::mode_l(int clientFd, bool addMode, std::string target, std::string target_user, std::string mode, std::map<std::string, Channel>::iterator match) {
	
	if (!_clients[clientFd].getModeO()) {
		sendAndLog(clientFd, this->get_prefix(clientFd) + " 482 " + _clients[clientFd].getNick() + " " + target + " :Not enought privileges" + "\r\n");
		return;
	}
	
	if (target_user.empty() && addMode) {
		sendAndLog(clientFd, get_prefix(clientFd) + " 461 " + _clients[clientFd].getNick() + " " + target + " " + mode + " :Need more argument" + "\r\n");
		return;
	}

	std::istringstream iss(target_user);
	int limit;
	iss >> limit;
	
	//? Check overflow ?
	if (!addMode) {
		match->second.setModeL(addMode);
		match->second.setLimitValue(0);
		
		std::string tmp = ":" + _servername + " MODE " + target + " " + mode + "\r\n";
		sendAndLog(clientFd, tmp);
		match->second.broadcast(clientFd, *this, tmp, true);
		return;
	}
	if (addMode && (limit > 0)) {
		match->second.setModeL(addMode);
		match->second.setLimitValue(limit);
		
		std::string tmp = ":" + _servername + " MODE " + target + " " + mode + " " + target_user + "\r\n";
		sendAndLog(clientFd, tmp);
		match->second.broadcast(clientFd, *this, tmp, true);
		return;
	}
	else {
		sendAndLog(clientFd, get_prefix(clientFd) + " 461 " + _clients[clientFd].getNick() + " " + target + " " + mode + " :Invalid limit" + "\r\n");
		return;
	}
}

void Server::mode_t(int clientFd, bool addMode, std::string target, std::string mode, std::map<std::string, Channel>::iterator match) {
	
	if (!_clients[clientFd].getModeO()) {
		sendAndLog(clientFd, this->get_prefix(clientFd) + " 482 " + _clients[clientFd].getNick() + " " + target + " :Not enought privileges" + "\r\n");
		return;
	}
	match->second.setModeT(addMode);
	std::string msg = ":" + _servername + " MODE " + target + " " + mode + "\r\n";
	sendAndLog(clientFd, msg);
	match->second.broadcast(clientFd, *this, msg, true);
}

void Server::mode_o(int clientFd, std::string target, std::string target_user, bool addMode, std::string mode, std::map<std::string, Channel>::iterator match) {
	
	if (target_user.empty()) {
		sendAndLog(clientFd, get_prefix(clientFd) + " 461 " + _clients[clientFd].getNick() + " " + target + " " + mode + " :Need more argument" + "\r\n");
		return;
	}
	
	if (_clients[clientFd].getModeO()) {
		if (match->second.isUserInChannel(target_user)) {
			
			Client *target_in_channel = match->second.getTarget(target_user);
			target_in_channel->setModeO(addMode);
			
			std::string tmp = ":" + _servername + " MODE " + target + " " + mode + " " + target_user + "\r\n";
			sendAndLog(clientFd, tmp);
			match->second.broadcast(clientFd, *this, tmp, true);
			return;
		}
		else {
			sendAndLog(clientFd, get_prefix(clientFd) + " 401 " + _clients[clientFd].getNick() + " " + target_user + " :No such nick in channel" + "\r\n");
			return;
		}
	}
	else {
		sendAndLog(clientFd, this->get_prefix(clientFd) + " 482 " + _clients[clientFd].getNick() + " " + target + " :Not enought privileges" + "\r\n");
		return;
	}
}

void Server::mode_i(int clientFd, bool addMode, std::string target, std::string mode, std::map<std::string, Channel>::iterator match) {

	if (_clients[clientFd].getModeO()) {
		
		match->second.setModeI(addMode);
		std::string tmp = ":" + _servername + " MODE " + target + " " + mode + "\r\n";
		sendAndLog(clientFd, this->get_prefix(clientFd) + " MODE " + target + " " + mode + "\r\n");
		match->second.broadcast(clientFd, *this, tmp, true);
		return;
	}
	else {
		sendAndLog(clientFd, this->get_prefix(clientFd) + " 482 " + _clients[clientFd].getNick() + " " + target + " :Not enought privileges" + "\r\n");
		return;
	}
}

bool Server::is_channel(std::string target) {
	
	std::map<std::string, Channel>::iterator match = _channels.begin();
	
	for (; match != _channels.end(); ++match) {
		if (match->second.getChanName() == target)
			return true;
	}
	return false;
}

void Server::case_mode_channel(std::string target, std::string mode, std::string target_user, int clientFd) {
	
	if (mode.empty()) {
		sendAndLog(clientFd, this->get_prefix(clientFd) + " 324 " + _clients[clientFd].getNick() + " " + target + "\r\n");
		return;		
	}
	
	std::map<std::string, Channel>::iterator match = _channels.begin();
	bool addMode = (mode[0] == '+');
	
	for(; match != _channels.end(); ++match) {
		if (match->second.getChanName() == (target)) {
			switch (mode[1]) {
				case 'i':
					mode_i(clientFd, addMode, target, mode, match);
					return;
				case 't':
					mode_t(clientFd, addMode, target, mode, match);
					return;
				case 'k':
					mode_k(clientFd, addMode, target, target_user, mode, match);
					return;
				case 'o':
					mode_o(clientFd, target, target_user, addMode, mode, match);
					return;
				case 'l':
					mode_l(clientFd, addMode, target, target_user, mode, match);
					return;
				default:
					sendAndLog(clientFd, this->get_prefix(clientFd) + " 472 " + _clients[clientFd].getNick() + " " + target + " :Unknown mode\r\n");
					return;
			}
		}
	}
	sendAndLog(clientFd, this->get_prefix(clientFd) + " 403 " + _clients[clientFd].getNick() + " " + mode + " :No such channel\r\n");
}

void Server::case_mode_user(std::string target, std::string mode, int clientFd) {
	
	// std::cout << BOLD_ON << "mode USER" << BOLD_OFF << std::endl;
	if ((mode[0] != '+' && mode[0] != '-') || mode.empty())
		//!error
		std::cout << YELLOW << "[WRGANSWER]: trouble +/-!" << BOLD_OFF << std::endl;
	
	bool add_mode = (mode[0] == '+');
	std::map<int, Client>::iterator match = _clients.begin();
	
	for (; match != _clients.end(); ++match) {

		if (match->second.getNick() == target) {
			
			for (size_t i = 1; i < mode.size(); ++i) {
				switch (mode[i]) {
					case 'i':
						match->second.setModeI(add_mode);
						break;
					default:
						sendAndLog(clientFd, this->get_prefix(clientFd) + " 472 " + _clients[clientFd].getNick() + " " + target + " :Unknown mode\r\n");
						return;
						break;
				}
			}
		}
		sendAndLog(clientFd, this->get_prefix(clientFd) + " MODE " + target + " " + mode + "\r\n");
		return;
	}
	sendAndLog(clientFd, this->get_prefix(clientFd) + " 401 " + _clients[clientFd].getNick() + " " + target + " :No such nick\r\n");
}

bool Server::is_user(std::string target) {

	std::map<int, Client>::iterator match = _clients.begin();
	for (; match != _clients.end(); ++match) {
		if (match->second.getNick() == target)
			return true;
	}
	return false;
}

void Server::mode(int clientFd, std::string cmd) {

	std::istringstream iss(cmd);
	std::string command, target, mode, target_user;
	iss >> command >> target >> mode >> target_user;

	std::cout << command << "|" << target << "|" << mode << "|" << target_user << std::endl;
	
	if (target[0] == '#')
		case_mode_channel(target, mode, target_user, clientFd);
	else if (is_user(target))
	 	case_mode_user(target, mode, clientFd);
	else
		//!error
		(void)clientFd;
}

