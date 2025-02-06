/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cyferrei <cyferrei@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/06 14:54:25 by cyferrei          #+#    #+#             */
/*   Updated: 2025/02/06 17:30:13 by cyferrei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Client.hpp"
#include "../includes/colors.hpp"

#include <sstream>
#include <iostream>
#include <string>

Client::Client(int client_fd, std::string msgFromClient) {

	(void)client_fd;
	(void)msgFromClient;
	
	std::istringstream stream(msgFromClient);
	std::string line;

	while(std::getline(stream, line)) {
		if (line.empty())
			continue;

		std::cout << ORANGE << "@serveur: Processing line: " << line << BOLD_OFF << std::endl;
		std::istringstream line_stream(line);
		std::string cmd;
		line_stream >> cmd;

		if (cmd == "CAP")
			std::cout << ORANGE << "@serveur: ignored" << BOLD_OFF << std::endl;
		else if (cmd == "NICK") {
			std::string nickname;
			line_stream >> nickname;
			this->_nick = nickname;
		}
		else if (cmd == "USER") {
			std::string username, host, serveur, realname;
			line_stream >> username >> host >> serveur;
			std::getline(line_stream, realname);
			if (!realname.empty() && realname[0] == ' ')
				realname.erase(0, 1);
			this->_user = username;
			this->_host = host;
			this->_real_name = realname;
		}
		else
			std::cout << RED << "@serveur: not handle or unknown cmd!" << BOLD_OFF << std::endl;
	}
}

void Client::setNick(std::string nick) {
	this->_nick = nick;
}

std::string Client::getNick(void) const{
	return this->_nick;
}

Client::~Client() {};
