/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_mode.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cyferrei <cyferrei@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/13 10:38:25 by cyferrei          #+#    #+#             */
/*   Updated: 2025/02/14 11:17:45 by cyferrei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Server.hpp"
#include "../includes/colors.hpp"

#include <sstream>
#include <iostream>
#include <sys/socket.h>

void Server::mode(int clientFd, std::string cmd) {

	std::istringstream iss(cmd);
	std::string command, nickname, mode;
	iss >> command >> nickname >> mode;

	if (mode.empty() || (mode[0] != '+' && mode[0] != '-')) {
		std::cout << RED << "ERROR in MODE: Invalid format (must start with + or -)" << BOLD_OFF << std::endl;
		return;
	}

	bool addMode = (mode[0] == '+');

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
				std::cout << YELLOW << "ERROR in MODE: Unknown mode " << mode[i] << "'" << BOLD_OFF << std::endl;
				break;
		}
	}
	if (nickname == _clients[clientFd].getNick())
		sendAndLog(clientFd, this->get_prefix(clientFd) + " " + "MODE " + _clients[clientFd].getUser() + " " + mode + "\r\n");
	else
		std::cout << "ERROR: Nickname mismatch" << std::endl;
}
