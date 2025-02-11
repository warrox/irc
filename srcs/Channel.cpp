/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: whamdi <whamdi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/10 11:43:54 by whamdi            #+#    #+#             */
/*   Updated: 2025/02/11 15:17:56 by whamdi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Channel.hpp"
#include "../includes/Server.hpp"
#include <algorithm>

Channel::Channel(std::string name, int fd) {
	this->_chan_name = name;
	this->_users.push_back(fd);
}

bool Channel::addUser(int fd) {
	if (std::find(this->_users.begin(), this->_users.end(), fd) == this->_users.end()) {
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
	for (it = this->_users.begin(); it != this->_users.end(); ++it) {
		if (*it == clientToIgnore) 
			continue;
		server.sendAndLog(*it, msg);
	}
}
