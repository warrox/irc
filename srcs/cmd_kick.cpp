/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_kick.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cyferrei <cyferrei@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/19 10:43:16 by whamdi            #+#    #+#             */
/*   Updated: 2025/02/24 18:14:18 by cyferrei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Server.hpp"
#include <cstdio>
#include <sstream>
#include <iostream>
#include <sys/socket.h>

void Server::kick(int clientFd, std::string cmd){
	// std::cout << "Command kick : " << cmd << std::endl;
	std::istringstream lineStream(cmd);	
	std::string cmdName, chan_name,client_to_kick, reason;
	lineStream >> cmdName;
	lineStream >> chan_name;
	lineStream >> client_to_kick;
	bool is_in_chan = false;
	if(this->_channels[chan_name].isUserInChannel(client_to_kick))
		is_in_chan = true;
	getline(lineStream,reason);
	if(this->_clients[clientFd].getModeO() && client_to_kick == this->_clients[clientFd].getNick())
	{

		std::string no_perm;
		no_perm = ":ft_irc 485 " + this->_clients[clientFd].getNick() + " :can't kick channel operator\r\n";
		this->sendAndLog(clientFd, no_perm);
	}
	else if(this->_clients[clientFd].getModeO())
	{
		if(!is_in_chan)
			return;
		std::string kick_answer;
		kick_answer = get_prefix(clientFd) + " KICK " +  chan_name + " " + client_to_kick +  " ";
		if(reason.size() == 2)
			kick_answer += "Kicked by an operator";
		else
			kick_answer += reason;
		kick_answer += "\r\n";
		this->sendAndLog(clientFd, kick_answer);
		this->_channels[chan_name].broadcast(clientFd, *this, kick_answer,true);
	}else{
		std::string kick_not_allowed;
		kick_not_allowed += "482 " + chan_name + " :Not enough privilege"; 
		this->sendAndLog(clientFd, kick_not_allowed);
	}
	
}
