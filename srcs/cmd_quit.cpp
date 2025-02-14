/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_quit.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cyferrei <cyferrei@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/13 18:17:06 by cyferrei          #+#    #+#             */
/*   Updated: 2025/02/14 10:15:24 by cyferrei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Server.hpp"

#include <unistd.h>

void	Server::quit(int clientFd, std::string cmd) {
	(void)cmd;
	std::string quitMsg = ":" + _clients[clientFd].getNick() + "!~" + _clients[clientFd].getUser() + "@" + _clients[clientFd].getHost() + " QUIT :Client Quit\r\n";
	
	// 	if (_clients.find(clientFd) != _clients.end()) {
	// 	sendAndLog(clientFd, ":" + this->_servername + " 421 " + _clients[clientFd].getNick() + " " + cmd + " :Unknown command\r\n");
	// }
	sendAndLog(clientFd, quitMsg);
	close(clientFd);
	_clients.erase(clientFd);
}