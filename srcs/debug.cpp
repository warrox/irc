#include "../includes/Server.hpp"
#include "../includes/colors.hpp"

#include <cstdlib>
#include <iostream>

void Server::sendAndLog(int clientFd, std::string msg) {
	std::cout << BOLD_ON RED << "[S -> " << clientFd << "]: " << BOLD_OFF << msg << std::endl;
	send(clientFd, msg.c_str(), msg.length(), 0);
}

void Server::log(std::string info) {
	std::cout << BOLD_ON GREEN "[INFO]: " BOLD_OFF << info << std::endl;
}

void Server::recvLog(int clientFd, std::string message) {
	std::cout << BOLD_ON BLUE << "[S <- " << clientFd <<  "]: " << BOLD_OFF << message;
}

//All exitfailure in this program should be replaced by a function that
//Close clients sever, destroy the server and leave
void Server::fatal(std::string error) {
	std::cerr << __FILE_NAME__ << ":" << __LINE__ << ": Fatal: " << error << std::endl;
	exit(EXIT_FAILURE);
}
