/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_pass.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cyferrei <cyferrei@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/13 10:43:20 by cyferrei          #+#    #+#             */
/*   Updated: 2025/02/19 00:27:39 by cyferrei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Server.hpp"

#include <sstream>

void Server::pass(int clientFd, std::string cmd) {

	std::istringstream line(cmd);
	std::string commandName;
	std::string password;
	
	line >> commandName;
	line >> password;

	this->_clients[clientFd].setPassword(password);
}
