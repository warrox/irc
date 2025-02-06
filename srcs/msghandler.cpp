#include "../includes/server.hpp"
#include <sstream>
#include <iostream>
#include <string>

void sendWelcomeMessage(int client_fd, std::string nick)
{
    std::string welcome = ":localhost 001 " + nick + " :Bienvenue sur mon serveur IRC !\r\n";
    send(client_fd, welcome.c_str(), welcome.length(), 0);
}

// void msgHandler::handlerMessage(int client_fd, std::string msgFromClient)
// {	
// 	if(msgFromClient.find("WHOIS") != std::string::npos)
// 	{
// 		std::cout << "WHOIS detected" << std::endl;	
// 		//handle
// 		std::string whois_response = 
// 			":server 311 warren warren ~user 127.0.0.1 * :Warren User\n"
// 			":server 312 warren irc.example.com :Example IRC Server\n"
// 			":server 318 warren :End of /WHOIS list.\n";
// 		send(client_fd, whois_response.c_str(), whois_response.size(), 0);
// 	}
// 	// and so on
// } 


void server::nick(int clientFd, std::string cmd) {
	(void)clientFd; (void)cmd;
	std::cout << "Hello from Nick" << std::endl;
}


void server::commandHandler(int clientFd, std::string cmd) {

	std::istringstream stream(cmd);
	std::string line;


	while(std::getline(stream, line)) {
		std::istringstream lineStream(line);
		std::string commandName;
		lineStream >> commandName;

		std::cout << "RECEIVED COMMAND |" << commandName << "|" << std::endl;

		std::map<std::string, CommandFunc>::iterator match = this->_commands.find(commandName);
		if (match != this->_commands.end()) {
			(this->*_commands[commandName])(clientFd, cmd);
		} else {
			std::cout << "Unhandled command" << std::endl;
		}
	}

	return ;
}