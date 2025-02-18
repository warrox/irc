/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_privmsg.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cyferrei <cyferrei@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/13 10:45:59 by cyferrei          #+#    #+#             */
/*   Updated: 2025/02/18 14:19:40 by whamdi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Server.hpp"

#include <sstream>
#include <iostream>
int Server::findClientByName(std::string name, std::map<int,Client> client)
{
	for (std::map<int, Client>::const_iterator it = client.begin(); it != client.end(); ++it) {
		if (it->second.getNick() == name)
			return(it->first);
	}
	return(-1);
}
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
		std::cout << message << std::endl;
		it->second.broadcast(clientFd, *this, message,false); //ici 
	}
	else{
		//je cherche le fd du client destinataire
		//j'ai comme information, le 'nick' de ce client
		//comment l'obtenir ?
		clientIterator it = this->_clients.begin();
		int fdOfReceiver = findClientByName(dest,this->_clients);
		if(fdOfReceiver == -1)
		{
			std::cerr << "Client not found" << std::endl;
			return;
		}
		for(; it != this->_clients.end(); ++it)
		{
			if(it->second.getNick() == dest) {
				// this->sendAndLog(it->first, message);
				this->sendMessageto(clientFd, fdOfReceiver, message, this->getClient(fdOfReceiver).getNick(), dest);
			}
		}
	}
	(void)clientFd;
	// clientIterator it = _clients.begin();
}
