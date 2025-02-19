/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cyferrei <cyferrei@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/10 11:43:54 by whamdi            #+#    #+#             */
/*   Updated: 2025/02/19 17:12:05 by cyferrei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Channel.hpp"
#include "../includes/Server.hpp"

#include <string>
#include <algorithm>
#include <iostream>
#include <vector>

// #include <sstream>

#include "../includes/Channel.hpp"
#include "../includes/Server.hpp"

void	Channel::setModeI(bool enable) {
	this->_modeI = enable;
}

bool	Channel::getModeI(void)const {
	return (this->_modeI);
}


std::vector<Client> Channel::getUsers(void)
{
    std::vector<Client> clientsInChan;
    std::map<int, Client> allClients = this->_server->getClients();

    for (std::vector<int>::iterator itUser = this->_users.begin(); itUser != this->_users.end(); ++itUser) {
        std::map<int, Client>::iterator itClient = allClients.find(*itUser);
        if (itClient != allClients.end())
            clientsInChan.push_back(itClient->second);
    }
    return clientsInChan;
}

void Channel::printClientInChan(std::vector<int> user)
{
	if(user.empty())
	{
		std::cout << "EMPTY" << std::endl;
		return;
	}
	for(size_t i = 0; i < user.size();i++)
	{
		std::cout<< "user in Channel : " << user[i] << std::endl;
	}
}
Channel::Channel(std::string name, int fd, Server &server) {

	this->_chan_name = name;
	(void)fd;
	this->_server = &server;
	this->_topic = "";
}


bool Channel::addUser(int fd) {
    if (std::find(this->_users.begin(), this->_users.end(), fd) != this->_users.end()) {
        return false;
    }

    this->_users.push_back(fd);
    return true;
}

bool Channel::delUser(int fd) {
	std::vector<int>::iterator it;
	if ((it = std::find(this->_users.begin(), this->_users.end(), fd)) != this->_users.end()) {
		this->_users.erase(it);
		return true;
	}
	return false;
}

void Channel::setTopic(std::string topic) {
	this->_topic = topic;
}

std::string Channel::getTopic(void){

	return(this->_topic);
}

void Channel::broadcast(int clientToIgnore, Server &server, std::string msg, bool is_for_topic) {

	std::vector<int>::iterator it;
	printClientInChan(this->_users);
	for (it = this->_users.begin(); it != this->_users.end(); ++it) {
		if (*it == clientToIgnore) 
			continue;
		// server.sendAndLog(*it, msg += "\r\n");
		if(is_for_topic){
			server.sendAndLog(*it, msg);
		}else{
			server.sendMessageto(clientToIgnore,*it, msg, server.getClient(clientToIgnore).getNick(), this->getChanName()); 
		}
	}
}

std::string Channel::getChanName(void) {
	return(this->_chan_name);
}

/********************************************************************************************************************************* */

void Channel::setModeOInChannel(std::string target_name, bool enable) {
	
	std::vector<Client>::iterator match = this->_usersInChannel.begin();
	
	for(; match != this->_usersInChannel.end(); ++match) {
		if (match->getNick() == target_name) {
			std::cout << "HEY" << std::endl;
			std::cout << match->getNick() << std::endl;
			match->setModeO(enable);
		}
	}
}

bool Channel::isUserInChannel(std::string target_name) {
	
	std::vector<Client>::iterator match = this->_usersInChannel.begin();
	for (; match != this->_usersInChannel.end(); ++match) {
		if (match->getNick() == target_name)
			return true;
	}
	return false;
}

void Channel::removeUserInChannel(Client client) {
	
	std::vector<Client>::iterator match = this->_usersInChannel.begin();
	for (; match != this->_usersInChannel.end(); ++match) {
		if (match->getNick() == client.getNick()) {
			this->_usersInChannel.erase(match);
			return;
		}
	}
}

void Channel::addUserInChannel(Client &client) {

	this->_usersInChannel.push_back(client);
}