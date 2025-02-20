/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_invite.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cyferrei <cyferrei@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/20 10:39:32 by whamdi            #+#    #+#             */
/*   Updated: 2025/02/20 15:44:14 by cyferrei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Server.hpp"

#include <sstream>
#include <sys/socket.h>

void Server::invite(int clientFd, std::string cmd)
{
	std::istringstream lineStream(cmd);	
	std::string cmdName, name_invit, chan_name; 
	lineStream >> cmdName;
	lineStream >> name_invit;
	lineStream >> chan_name;
	int fdOfreceiver = 0;
	fdOfreceiver = findClientByName(name_invit, this->_clients);
	if(fdOfreceiver== -1){
		this->log("No such a nick name in the client base\n");
		return;
	}
	if(this->_clients[clientFd].getModeO())
	{
		std::string invite_answer;
		invite_answer += ":ft_irc 341 " + this->_clients[clientFd].getNick() + " "+ name_invit + " " + chan_name + "\r\n";
		this->sendAndLog(fdOfreceiver, invite_answer);
		this->_channels[chan_name].client_invite_lst.push_back(name_invit);
	}		
}
