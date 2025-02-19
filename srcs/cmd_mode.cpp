/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_mode.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cyferrei <cyferrei@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/13 10:38:25 by cyferrei          #+#    #+#             */
/*   Updated: 2025/02/19 16:38:31 by cyferrei         ###   ########.fr       */
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

// bool Server::isUserInChannel(std::string channel_name, std::string target_name) {
// 	this->_channels[channel_name].
// }

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
	(void)addMode;
	
	for(; match != _channels.end(); ++match) {
		
		// std::cout << "INSIDE" << std::endl;
		// std::cout << "DEBUG :" << match->second.getChanName() << std::endl;
		// std::cout << "DEBUG :" << (target) << std::endl;
		if (match->second.getChanName() == (target)) {
			for (size_t i = 1; i < mode.size(); ++i) {
				switch (mode[i]) {
					case 'i':
						// std::cout << BOLD_ON << "MODE I" << BOLD_OFF << std::endl;
						if (_clients[clientFd].getModeO()) {
							match->second.setModeI(addMode);
							sendAndLog(clientFd, this->get_prefix(clientFd) + " MODE " + target + " " + mode + "\r\n");
							return;
						}
						else {
							sendAndLog(clientFd, this->get_prefix(clientFd) + " 482 " + _clients[clientFd].getNick() + " " + target + " :Not enought privileges" + "\r\n");
							return;
						}
						break;
					case 't':
						std::cout << BOLD_ON << "MODE T" << BOLD_OFF << std::endl;
						break;
					case 'k':
						std::cout << BOLD_ON << "MODE K" << BOLD_OFF << std::endl;
						break;
					case 'o':
						if (_clients[clientFd].getModeO())
							std::cout << BOLD_ON GREEN << "MODE O" << BOLD_OFF << std::endl;
						if (target_user.empty()) {
							sendAndLog(clientFd, get_prefix(clientFd) + " 461 " + _clients[clientFd].getNick() + " " + target + " " + mode + " :Need more argument" + "\r\n");
							return;
						}
						if (_clients[clientFd].getModeO()) {
							if (match->second.isUserInChannel(target_user)) {
								match->second.setModeOInChannel(target_user, addMode);
								sendAndLog(clientFd, ":" + _servername + " MODE " + target + " " + mode + " " + target_user + "\r\n");
								//? need to broadcast
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
						break;
					case 'l':
						std::cout << BOLD_ON << "MODE L" << BOLD_OFF << std::endl;
						break;
					default:
						sendAndLog(clientFd, this->get_prefix(clientFd) + " 472 " + _clients[clientFd].getNick() + " " + target + " :Unknown mode\r\n");
						return;
				}
			}
		}
	}
	sendAndLog(clientFd, this->get_prefix(clientFd) + " 403 " + _clients[clientFd].getNick() + " " + mode + " :No such channel\r\n");
}

void Server::case_mode_channel_response(std::string target, int clientFd) {
	
	sendAndLog(clientFd, this->get_prefix(clientFd) + " 324 " + _clients[clientFd].getNick() + " " + target + "\r\n");
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
// void Server::mode(int clientFd, std::string cmd) {
	
// 	std::istringstream iss(cmd);
// 	std::string command, target, mode, targetUser;
// 	iss >> command >> target >> mode >> targetUser;

// 	//requete sans modifications
// 	if (mode.empty()) {
// 		if (_channels.find(target) != _channels.end()) {
// 			std::string currentModes = _channels[target].getModeChan();
// 			std::string creationTime = _channels[target].getCreationTime();
			
// 			sendAndLog(clientFd, this->get_prefix(clientFd) + " 324 " + _clients[clientFd].getNick() + " " + target + " " + currentModes + "\r\n");
// 			sendAndLog(clientFd, this->get_prefix(clientFd) + " 329 " + _clients[clientFd].getNick() + " " + target + " " + creationTime + "\r\n");
// 		} else
// 			sendAndLog(clientFd, this->get_prefix(clientFd) + " 403 " + _clients[clientFd].getNick() + " " + target + " :No such channel\r\n");
// 		return;
// 	}

// 	// mini check syntax
// 	if (mode.empty() || (mode[0] != '+' && mode[0] != '-')) {
// 		std::cout << RED << "ERROR in MODE: Invalid format (must start with + or -)" << BOLD_OFF << std::endl;
// 		return;
// 	}

// 	bool addMode = (mode[0] == '+');
	
// 	// Si target est un canal
// 	if (_channels.find(target) != _channels.end()) {
// 		Channel &channel = _channels[target];

// 		// Vérifier si l'expéditeur est opérateur du canal
// 		if (!channel.isOperator(clientFd)) {
// 			sendAndLog(clientFd, this->get_prefix(clientFd) + " 482 " + _clients[clientFd].getNick() + " " + target + " :You're not channel operator\r\n");
// 			return;
// 		}

// 		for (size_t i = 1; i < mode.size(); ++i) {
// 			switch (mode[i]) {
// 				case 'i':
// 					channel.setModeI(addMode);
// 					break;
// 				case 't':
// 					channel.setModeT(addMode);
// 					break;
// 				case 'k':
// 					// Vérifie qu'une clé est fournie après `+k`
// 					if (addMode && targetUser.empty()) {
// 						sendAndLog(clientFd, this->get_prefix(clientFd) + " 461 " + _clients[clientFd].getNick() + " MODE :Not enough parameters\r\n");
// 						return;
// 					}
// 					channel.setModeK(addMode, targetUser);
// 					break;
// 				case 'o':
// 					// Vérifie qu'un utilisateur est spécifié pour +o / -o
// 					if (targetUser.empty()) {
// 						sendAndLog(clientFd, this->get_prefix(clientFd) + " 461 " + _clients[clientFd].getNick() + " MODE :Not enough parameters\r\n");
// 						return;
// 					}
// 					// Vérifie si l'utilisateur ciblé est dans le canal
// 					if (!channel.hasClient(targetUser)) {
// 						sendAndLog(clientFd, this->get_prefix(clientFd) + " 441 " + _clients[clientFd].getNick() + " " + targetUser + " " + target + " :They aren't on that channel\r\n");
// 						return;
// 					}
// 					// Ajoute ou retire l'opérateur sur l'utilisateur ciblé
// 					channel.setOperator(clientFd, addMode);
// 					break;
// 				default:
// 					std::cout << YELLOW << "ERROR in MODE: Unknown mode '" << mode[i] << "'" << BOLD_OFF << std::endl;
// 					break;
// 			}
// 		}
// 		sendAndLog(clientFd, this->get_prefix(clientFd) + " MODE " + target + " " + mode + " " + targetUser + "\r\n");
// 	}

// 	// Si target est un utilisateur
// 	else {
// 		// Recherche dans _clients en utilisant le nick exact de target
// 		std::map<int, Client>::iterator it = _clients.begin();
// 		for (; it != _clients.end(); ++it) {
// 			if (it->second.getNick() == target) {
// 				Client &targetClient = it->second;
// 				std::cout << "HERE :" + targetClient.getNick() << std::endl;

// 				for (size_t i = 1; i < mode.size(); ++i) {
// 					switch (mode[i]) {
// 						case 'i':
// 							it->second.setModeI(addMode);
// 							break;
// 						case 't':
// 							it->second.setModeT(addMode);
// 							break;
// 						case 'k':
// 							it->second.setModeK(addMode);
// 							break;
// 						case 'o':
// 							it->second.setModeO(addMode);
// 							break;
// 						default:
// 							std::cout << YELLOW << "ERROR in MODE: Unknown mode '" << mode[i] << "'" << BOLD_OFF << std::endl;
// 							break;
// 					}
// 				}
// 				std::cout << mode << std::endl;
// 				sendAndLog(clientFd, this->get_prefix(clientFd) + " MODE " + target + " " + mode + "\r\n");
// 				// displayClientsInfo();
// 				return;
// 			}
// 		}
// 		// Si l'utilisateur n'est pas trouvé
// 		sendAndLog(clientFd, this->get_prefix(clientFd) + " 401 " + _clients[clientFd].getNick() + " " + target + " :No such nick/channel\r\n");
// 	}
// }
