/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_privmsg.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cyferrei <cyferrei@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/13 10:45:59 by cyferrei          #+#    #+#             */
/*   Updated: 2025/02/13 10:46:30 by cyferrei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Server.hpp"

#include <sstream>

void Server::privmsg(int clientFd, std::string cmd)
{
	std::istringstream lineStream(cmd);
	std::string cmdName, dest, message;
	lineStream >> cmdName;
	lineStream >> dest;
	lineStream >> cmdName;
	std::getline(lineStream, message);


	if (dest.at(0) == '#') {
		channelIterator it = this->_channels.find(dest);
		if (it == this->_channels.end())
			return ;
		this->log("Broadcasting on channel: " + dest);
		it->second.broadcast(clientFd, *this, message);
	}
	else{
		//je cherche le fd du client destinataire
		//j'ai comme information, le 'nick' de ce client
		//comment l'obtenir ?
		clientIterator it = this->_clients.begin();
		for(; it != this->_clients.end(); ++it)
		{
			if(it->second.getNick() == dest) {
				this->sendAndLog(it->first, message);
			}
		}
	}
	(void)clientFd;
	// clientIterator it = _clients.begin();
}