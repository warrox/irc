#include "../includes/msghandler.hpp"
#include <sys/socket.h>
#include <iostream>
msgHandler::msgHandler(std::string msg)
{
	this->_msg = msg;
}

msgHandler::msgHandler(){}
msgHandler::~msgHandler(){};

void msgHandler::sendWelcomeMessage(int client_fd, std::string nick)
{
    std::string welcome = ":localhost 001 " + nick + " :Bienvenue sur mon serveur IRC !\r\n";
    send(client_fd, welcome.c_str(), welcome.length(), 0);
}

void msgHandler::handlerMessage(int client_fd, std::string msgFromClient)
{
	sendWelcomeMessage(client_fd, "guest");
	if(msgFromClient.find("WHOIS") != std::string::npos)
	{
		std::cout << "WHOIS detected" << std::endl;	
		//handle
	}
	// and so on
} 

