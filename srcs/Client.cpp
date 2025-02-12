/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cyferrei <cyferrei@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/06 14:54:25 by cyferrei          #+#    #+#             */
/*   Updated: 2025/02/12 13:57:04 by whamdi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Client.hpp"

#include <string>

void Client::setNick(std::string nick) {
	this->_nick = nick;
}

void	Client::setUser(std::string user) {
	this->_user = user;
}

void	Client::setHost(std::string host) {
	this->_host = host;
}

void	Client::setRealName(std::string realname) {
	this->_real_name = realname;
}

std::string Client::getNick(void) const{
	return this->_nick;
}

std::string Client::getUser(void)const{
	return this->_user;
}

std::string Client::getHost(void)const{
	return this->_host;
}

std::string Client::getRealName(void)const {
	return this->_real_name;
}


std::string	Client::getChan(void)
{
	if(this->_actual_chan != "NO")
	{
		return(this->_actual_chan);
	}
	return("NO");
}
void Client::setIsConnected(bool isConnected){this->_is_connected = isConnected;}
bool Client::getIsConnected(void)const {return this->_is_connected;}

void Client::setPassword(std::string password){this->_password = password;}
std::string Client::getPassword(void)const {return this->_password;}
//Same as server, if you don't need a destructor, initialize it in the header file.
//I doubt its usefullness here
/*Client::~Client() {};*/
