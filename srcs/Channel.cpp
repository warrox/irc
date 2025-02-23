/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cyferrei <cyferrei@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/10 11:43:54 by whamdi            #+#    #+#             */
/*   Updated: 2025/02/23 23:06:06 by cyferrei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Channel.hpp"
#include "../includes/Server.hpp"

#include <cstddef>
#include <string>
#include <algorithm>
#include <iostream>
#include <vector>

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
Channel::Channel(std::string name, Server &server) {

	this->_chan_name = name;
	this->_server = &server;
	this->_topic = "";
	
	this->_modeI = false;
	this->_modeL = false;
	this->_modeT = false;
	this->_modeK = false;
	this->_limitValue = 0;
	this->_keyChannel = "";
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
	// printClientInChan(this->_users);
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

Channel::Channel() {
	
	this->_chan_name = "";
	this->_server = NULL;
	
	this->_modeI = false;
	this->_modeL = false;
	this->_modeT = false;
	this->_modeK = false;
	this->_limitValue = 0;
	this->_keyChannel = "";
}

void Channel::addNameInListChannel(std::string name) {
	this->_userNameList.push_back(name);
}

void Channel::removeNameUserInChannel(std::string name) {
	
	std::vector<std::string>::iterator it = _userNameList.begin();
	while (it != _userNameList.end()) {
		if (*it == name)
			it = _userNameList.erase(it);
		else
			++it;
	}
	
}

size_t Channel::getNbUsersInChannel() {
	return this->_usersInChannel.size();
}

void Channel::setModeOInChannel(std::string target_name, bool enable) {
	
	std::vector<Client*>::iterator match = this->_usersInChannel.begin();
	
	for(; match != this->_usersInChannel.end(); ++match) {
		if ((*match)->getNick() == target_name) {
			// std::cout << "HEY" << std::endl;
			// std::cout << (*match)->getNick() << std::endl;
			(*match)->setModeO(enable);
		}
	}
}

bool Channel::isUserInChannel(std::string target_name) {
	
	std::vector<Client*>::iterator match = this->_usersInChannel.begin();
	for (; match != this->_usersInChannel.end(); ++match) {
		if ((*match)->getNick() == target_name)
			return true;
	}
	return false;
}

bool Channel::isUserInvitedInChannel(std::string invited) {
	
	std::vector<std::string>::iterator match = this->client_invite_lst.begin();
	for (; match != this->client_invite_lst.end(); ++match) {
		if (*match == invited)
			return true;
	}
	return false;
}


void Channel::removeUserInChannel(Client &client) {
	
	std::vector<Client*>::iterator match = this->_usersInChannel.begin();
	for (; match != this->_usersInChannel.end(); ++match) {
		if ((*match)->getNick() == client.getNick()) {
			this->_usersInChannel.erase(match);
			return;
		}
	}
}

Client *Channel::getTarget(std::string target) {
	
	for (std::vector<Client*>::iterator match = _usersInChannel.begin(); match != _usersInChannel.end(); ++match) {
		if ((*match)->getNick() == target)
			return (*match);
	}
	return NULL;
}

void Channel::addUserInChannel(Client &client) {

	this->_usersInChannel.push_back(&client);
}

void	Channel::setModeI(bool enable) {
	this->_modeI = enable;
}

void	Channel::setModeT(bool enable) {
	this->_modeT = enable;
}

void	Channel::setLimitValue(int value) {
	this->_limitValue = value;
}

void	Channel::setModeL(bool enable) {
	this->_modeL = enable;
}

void	Channel::setModeK(bool enable) {
	this->_modeK = enable;
}

void	Channel::setKeyChannel(std::string key) {
	this->_keyChannel = key;
}

bool	Channel::getModeI(void)const {
	return (this->_modeI);
}

bool	Channel::getModeT(void)const {
	return (this->_modeT);
}

bool	Channel::getModeL(void)const {
	return (this->_modeL);
}

int	Channel::getLimitValue(void)const {
	return (this->_limitValue);
}

bool	Channel::getModeK(void)const {
	return (this->_modeK);
}

std::string	Channel::getKeyChannel(void)const {
	return (this->_keyChannel);
}