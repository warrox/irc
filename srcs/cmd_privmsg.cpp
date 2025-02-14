/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_privmsg.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cyferrei <cyferrei@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/13 10:45:59 by cyferrei          #+#    #+#             */
/*   Updated: 2025/02/13 15:33:44 by whamdi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Server.hpp"

#include <sstream>
#include <iostream>
void Server::privmsg(int clientFd, std::string cmd)
{
	std::istringstream lineStream(cmd);
	std::string cmdName, dest, message;
	lineStream >> cmdName;
	lineStream >> dest;
	std::string rest = lineStream.str();
	const size_t pos = rest.find(":");
	message = rest.substr(pos + 1, rest.size());
	if (dest.at(0) == '#') {
		channelIterator it = this->_channels.find(dest);
		if (it == this->_channels.end())
			return ;
		this->log("Broadcasting on channel: " + dest);
		it->second.broadcast(clientFd, *this, message); //ici 
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
