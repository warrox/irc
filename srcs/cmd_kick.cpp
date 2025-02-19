/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_kick.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cyferrei <cyferrei@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/19 10:43:16 by whamdi            #+#    #+#             */
/*   Updated: 2025/02/19 17:13:02 by cyferrei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Server.hpp"
#include <cstdio>
#include <sstream>
#include <iostream>
#include <sys/socket.h>

void Server::kick(int clientFd, std::string cmd){
	std::cout << "Command kick : " << cmd << std::endl;
	std::istringstream lineStream(cmd);	
	std::string cmdName, chan_name,client_to_kick, reason;
	lineStream >> cmdName;
	lineStream >> chan_name;
	lineStream >> client_to_kick;
	getline(lineStream,reason);
	// std::cout << "Size of :" << reason.size() << std::endl; // CHECK SIZE 
	// lineStream >> reason;
	// std::cout << "Bozo to kick : " << client_to_kick << std::endl;
	if(this->_clients[clientFd].getModeO())
	{
		// send and kick from channel		
		std::string kick_answer;
		kick_answer = get_prefix(clientFd) + " KICK " +  chan_name + " " + client_to_kick +  " ";
		if(reason.size() == 2)
			kick_answer += "Kicked by an operator";
		else
			kick_answer += reason;
		kick_answer += "\r\n";
		this->sendAndLog(clientFd, kick_answer);
		// std::cout << "DOuble bozlard : " << kick_answer << std::endl;
	}else{
		// t'as pas les droit et tu refuses
		std::string kick_not_allowed;
		kick_not_allowed += "482 " + chan_name + " :Not enough privilege"; 
		// send(clientFd,kick_not_allowed.c_str(), kick_not_allowed.size(),0);	
		this->sendAndLog(clientFd, kick_not_allowed);
	}
	
}
