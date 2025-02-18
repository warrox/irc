/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cyferrei <cyferrei@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/10 11:43:54 by whamdi            #+#    #+#             */
/*   Updated: 2025/02/18 11:37:01 by cyferrei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Channel.hpp"
#include "../includes/Server.hpp"

#include <string>
#include <algorithm>
#include <iostream>
#include <vector>

#include <sstream>

#include "../includes/Channel.hpp"
#include "../includes/Server.hpp"

// std::string Channel::getCreationTime() const {

// 	std::ostringstream oss;
// 	oss << _creationTime;
// 	return oss.str();
// }

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

void Channel::broadcast(int clientToIgnore, Server &server, std::string msg) {

	std::vector<int>::iterator it;
	printClientInChan(this->_users);
	for (it = this->_users.begin(); it != this->_users.end(); ++it) {
		if (*it == clientToIgnore) 
			continue;
		// server.sendAndLog(*it, msg += "\r\n");
		server.sendMessageto(clientToIgnore,*it, msg, server.getClient(clientToIgnore).getNick(), this->getChanName()); 
	}
}

std::string Channel::getChanName(void) {
	return(this->_chan_name);
}
