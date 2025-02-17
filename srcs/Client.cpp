/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cyferrei <cyferrei@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/17 13:58:06 by cyferrei          #+#    #+#             */
/*   Updated: 2025/02/17 13:58:16 by cyferrei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Client.hpp"

#include <string>

Client::Client() {
	
	this->_password = "";

	this->mode_i = false;
	this->mode_k = false;
	this->mode_o = false;
	this->mod_t = false;
	this->mode_l = false;
};

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

void	Client::setModeI(bool enable) {
	this->mode_i = enable;
}

void	Client::setModeT(bool enable) {
	this->mod_t = enable;
}

void	Client::setModeK(bool enable) {
	this->mode_k = enable;
}

void	Client::setModeO(bool enable) {
	this->mode_o = enable;
}

void	Client::setModeL(bool enable) {
	this->mode_l = enable;
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

bool Client::getModeI(void)const {
	return (this->mode_i);
}

bool Client::getModeT(void)const {
	return (this->mod_t);
}

bool Client::getModeK(void)const {
	return (this->mode_k);
}

bool Client::getModeO(void)const {
	return (this->mode_o);
}

bool Client::getModeL(void)const {
	return(this->mode_l);
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

