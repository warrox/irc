/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   debug.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cyferrei <cyferrei@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/13 10:48:41 by cyferrei          #+#    #+#             */
/*   Updated: 2025/02/14 16:07:33 by whamdi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "../includes/Server.hpp"
#include "../includes/colors.hpp"
#include "../includes/Client.hpp"
#include <cstdlib>
#include <iostream>

void Server::sendAndLog(int clientFd, std::string msg) {
	std::cout << BOLD_ON RED << "[S -> " << clientFd << "]: " << BOLD_OFF << msg;
	send(clientFd, msg.c_str(), msg.length(), 0);
}

void Server::log(std::string info) {
	std::cout << BOLD_ON GREEN "[INFO]: " BOLD_OFF << info << std::endl;
}

void Server::recvLog(int clientFd, std::string message) {
	std::cout << BOLD_ON BLUE << "[S <- " << clientFd <<  "]: " << BOLD_OFF << message;
}

//All exitfailure in this program should be replaced by a function that
//Close clients sever, destroy the server and leave
void Server::fatal(std::string error) {
	std::cerr << __FILE_NAME__ << ":" << __LINE__ << ": Fatal: " << error << std::endl;
	exit(EXIT_FAILURE);
}

void Server::displayClientsInfo(void){

	for (std::map<int, Client>::iterator it = this->_clients.begin(); it != this->_clients.end(); ++it) {
		std::cout << BLUE << "[_nick]--> " << BOLD_OFF << it->second.getNick() << std::endl;
		std::cout << BLUE << "[_user]--> " << BOLD_OFF << it->second.getUser() << std::endl;
		std::cout << BLUE << "[_host]--> " << BOLD_OFF << it->second.getHost() << std::endl;
		std::cout << BLUE << "[_real_name]--> " << BOLD_OFF << it->second.getRealName() << std::endl;

		std::cout << BLUE << "[mode_i]--> " << BOLD_OFF << (it->second.getModeI() ? "enabled" : "disabled") << std::endl;
		std::cout << BLUE << "[mode_t]--> " << BOLD_OFF << (it->second.getModeT() ? "enabled" : "disabled") << std::endl;
		std::cout << BLUE << "[mode_k]--> " << BOLD_OFF << (it->second.getModeK() ? "enabled" : "disabled") << std::endl;
		std::cout << BLUE << "[mode_o]--> " << BOLD_OFF << (it->second.getModeO() ? "enabled" : "disabled") << std::endl;
		std::cout << "-------------------" << std::endl;
	}
}
