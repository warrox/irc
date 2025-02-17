/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_mode.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cyferrei <cyferrei@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/13 10:38:25 by cyferrei          #+#    #+#             */
/*   Updated: 2025/02/17 17:56:43 by cyferrei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Server.hpp"
#include "../includes/colors.hpp"

#include <sstream>
#include <iostream>
#include <sys/socket.h>

void Server::mode(int clientFd, std::string cmd) {

	std::istringstream iss(cmd);
	std::string command, target_channel, mode, target_user;
	iss >> command >> target_channel >> mode >> target_user;

	std::cout << command << "|" << target_channel << "|" << mode << "|" << target_user << std::endl;
	
	//? if (target_channel.empty())
	//? 	sendAndLog(clientFd, this->get_prefix(clientFd) + " 461 " + _clients[clientFd].getNick() + " MODE :Not enough parameters\r\n");
	
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
