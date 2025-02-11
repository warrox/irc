/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: whamdi <whamdi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/10 11:43:54 by whamdi            #+#    #+#             */
/*   Updated: 2025/02/10 17:26:04 by whamdi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Channel.hpp"
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
