/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cyferrei <cyferrei@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/06 14:54:25 by cyferrei          #+#    #+#             */
/*   Updated: 2025/02/14 11:50:13 by whamdi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Client.hpp"

#include <string>

Client::Client() {
	
	this->_password = "";

	this->mode_i = false;
	this->mode_k = false;
	this->mode_o = false;
	this->mode_t = false;
};

		std::string _nick;
		std::string _user;
		std::string _host;
		std::string _real_name;
		std::string _password;
		
		bool mode_i;
		bool mode_t;
		bool mode_k;
		bool mode_o;
		bool _is_connected;
		
		std::string _actual_chan;


Client& Client::operator=(const Client &rhs)
{
	if(this != &rhs)
	{
		this->_nick = rhs._nick;
		this->_user = rhs._user;
		this->_host = rhs._host;
		this->_real_name = rhs._real_name;
		this->_password = rhs._password;
		this->mode_i = rhs.mode_i;
		this->mode_t = rhs.mode_t;
		this->mode_k = rhs.mode_k;
		this->mode_o = rhs.mode_o;
		this->_is_connected = rhs._is_connected;
		this->_actual_chan = rhs._actual_chan;
	}		
	return(*this);
}
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
	this->mode_t = enable;
}

void	Client::setModeK(bool enable) {
	this->mode_k = enable;
}

void	Client::setModeO(bool enable) {
	this->mode_o = enable;
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
	return (this->mode_t);
}

bool Client::getModeK(void)const {
	return (this->mode_k);
}

bool Client::getModeO(void)const {
	return (this->mode_o);
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

