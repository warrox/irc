/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_ping.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cyferrei <cyferrei@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/13 18:30:07 by cyferrei          #+#    #+#             */
/*   Updated: 2025/02/24 19:02:21 by cyferrei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Server.hpp"

#include <sstream>

void Server::ping(int clientFd, std::string cmd) {

	std::istringstream stream(cmd);
	std::string command, token;

	stream >> command >> token;

	if (token.empty()) 
		token = this->_servername;
	std::string response = "PONG " + token + "\r\n";
	sendAndLog(clientFd, response);
}