/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cyferrei <cyferrei@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/10 11:43:54 by whamdi            #+#    #+#             */
/*   Updated: 2025/02/17 16:17:20 by cyferrei         ###   ########.fr       */
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

std::string Channel::getModeChan()const {
	return this->_modesChannel;
}

// Vérifie si un client est opérateur du canal
bool Channel::isOperator(int clientFd) {
	if (_server->getClient(clientFd).getModeO())
		return (true);
	return (false);
}

bool Channel::hasClient(std::string targetUser) {
	
	std::vector<Client> tmp_vec_users = getUsers();
	for (std::vector<Client>::iterator match = tmp_vec_users.begin(); match != tmp_vec_users.end(); ++match) {
		if (match->getNick() == targetUser)
			return true;
	}
	return (false);
}

// Définit ou enlève le mode "invite-only" (+i)
void Channel::setModeI(bool enable) {
	_inviteOnly = enable;
}

// Définit ou enlève la restriction du topic (+t)
void Channel::setModeT(bool enable) {
	_topicRestricted = enable;
}

// Définit ou enlève une clé (+k)
void Channel::setModeK(bool enable, std::string key) {
	if (enable)
		_key = key;
	else
		_key.clear();
}

// Ajoute ou retire un opérateur du canal (+o / -o)
void Channel::setOperator(int clientFd, bool enable) {
	// Si on active le mode opérateur
	if (enable) {
		// Vérifie si le client n'est pas déjà un opérateur
		if (!isOperator(clientFd)) {
			// Ajouter l'opérateur à la liste des opérateurs
			_users.push_back(clientFd);
		}
	} else {
		// Si le mode est désactivé, on vérifie si le client est un opérateur
		if (isOperator(clientFd)) {
			// Supprimer l'opérateur de la liste des opérateurs
			_users.erase(std::remove(_users.begin(), _users.end(), clientFd), _users.end());
		}
	}
}


std::string Channel::getCreationTime() const {

	std::ostringstream oss;
	oss << _creationTime;
	return oss.str();
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
	
	this->_inviteOnly = false;
	this->_topicRestricted = false;
	this->_key = "";
	this->_creationTime = std::time(0);
	this->_modesChannel = "+nt";
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
