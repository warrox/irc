#include "../includes/server.hpp"
#include <iostream>
#include <cstring>
#include <sys/socket.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <cstdlib>
#include <poll.h>
#include <stdio.h>

int main(int argc, char **argv)
{
	if (argc < 3) {
		std::cout << "./ircserv <port> <password>" << std::endl;
		exit(EXIT_FAILURE);
	}

	int port = parserPort(argv[1]);
	//If the program exits here, why would you need a else statement
	//Removing the esle prevent from creating a scope unnecessarily
	if (port == -1)
		exit(EXIT_FAILURE);
	/*else*/
	/*{*/
		server server(argv[1],argv[2]);		
	/*}*/
	return(0);
}
