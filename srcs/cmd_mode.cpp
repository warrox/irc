/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_mode.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cyferrei <cyferrei@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/13 10:38:25 by cyferrei          #+#    #+#             */
/*   Updated: 2025/02/13 15:07:38 by cyferrei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Server.hpp"
#include "../includes/colors.hpp"

#include <sstream>
#include <iostream>
#include <sys/socket.h>

// void Server::mode(int clientFd, std::string cmd) {

// 	std::istringstream iss(cmd);
// 	std::string command, nickname, mode;
// 	iss >> command >> nickname >> mode;

// 	std::cout << "CMD: " << cmd << "'" << std::endl;

// 	if (mode.empty() || (mode[0] != '+' && mode[0] != '-')) {
// 		std::cout << RED << "ERROR in MODE: Invalid format (must start with + or -)" << BOLD_OFF << std::endl;
// 		return;
// 	}

// 	bool addMode = (mode[0] == '+');

// 	for (size_t i = 1; i < mode.size(); ++i) {
// 		switch (mode[i]) {
// 			case 'i':
// 				_clients[clientFd].setModeI(addMode);
// 				break;
// 			case 't':
// 				_clients[clientFd].setModeT(addMode);
// 				break;
// 			case 'k':
// 				_clients[clientFd].setModeK(addMode);
// 				break;
// 			case 'o':
// 				_clients[clientFd].setModeO(addMode);
// 				break;
// 			default:
// 				std::cout << YELLOW << "ERROR in MODE: Unknown mode " << mode[i] << "'" << BOLD_OFF << std::endl;
// 				break;
// 		}
// 	}
// 	if (nickname == _clients[clientFd].getNick())
// 	{
// 		std::cout << "CMD: '" << cmd << "'" << std::endl;
// 		sendAndLog(clientFd, ":" + _clients[clientFd].getNick() + "!" + _clients[clientFd].getUser() + "@" + _clients[clientFd].getHost() + " " + cmd + "\r\n");
// 	}
		
// 	else
// 		std::cout << "ERROR: Nickname mismatch" << std::endl;
// }

void Server::mode(int clientFd, std::string cmd) {
	// Affichage de la commande originale pour le débogage
	std::cout << "Original CMD: " << cmd << std::endl;

	// Extraction des paramètres de la commande
	std::istringstream iss(cmd);
	std::string command, nickname, mode;
	iss >> command >> nickname >> mode;

	// Affichage des valeurs extraites pour le débogage
	std::cout << "CMD: " << cmd << std::endl;
	std::cout << "Nickname: " << nickname << std::endl;
	std::cout << "Mode: " << mode << std::endl;

	// Vérification du format du mode (doit commencer par + ou -)
	if (mode.empty() || (mode[0] != '+' && mode[0] != '-')) {
		std::cout << RED << "ERROR in MODE: Invalid format (must start with + or -)" << BOLD_OFF << std::endl;
		return;
	}

	// Déterminer si on ajoute ou retire un mode
	bool addMode = (mode[0] == '+');

	// Application des modes pour chaque caractère
	for (size_t i = 1; i < mode.size(); ++i) {
		switch (mode[i]) {
			case 'i':
				_clients[clientFd].setModeI(addMode);
				break;
			case 't':
				_clients[clientFd].setModeT(addMode);
				break;
			case 'k':
				_clients[clientFd].setModeK(addMode);
				break;
			case 'o':
				_clients[clientFd].setModeO(addMode);
				break;
			default:
				std::cout << YELLOW << "ERROR in MODE: Unknown mode '" << mode[i] << "'" << BOLD_OFF << std::endl;
				break;
		}
	}

	// Vérification du surnom et envoi de la réponse
	if (nickname == _clients[clientFd].getNick()) {
		std::string response = ":" + _clients[clientFd].getNick() + "!" + _clients[clientFd].getUser() + "@" + _clients[clientFd].getHost() + " " + cmd + "\r\n";

		// Affichage de la commande envoyée pour le débogage
		std::cout << "Sending Response: '" << response << "'" << std::endl;

		sendAndLog(clientFd, response);
	}
	else {
		std::cout << "ERROR: Nickname mismatch" << std::endl;
	}
}
