#include "../includes/server.hpp" 
#include <cstdio>
#include <iostream>
#include <string.h>
#include <sstream>


int parserPort(char *portInString)
{
	for(size_t i = 0; i < strlen(portInString);i++)
	{
		if(!isdigit(portInString[i]))
		{
			std::cout << "Port error, please check and relaunch irc" << std::endl;
			return(-1);
		}
		if(i > 4)
		{
			std::cout << "Please use 6667,6660,6669 or 7000 for a standard non secure connexion" << std::endl;
			std::cout << "6697 or 7001 for a SSL/TLS secure connexion" << std::endl;
			return(-1);
		}
	}
	int port = 0;
	std::istringstream stream(portInString);
	stream>>port;
	if(port == 6667 || port == 6660 || port == 7000 || port == 6697 || port == 7001)
	{
		return(port);
	}	
	std::cout << "Please use 6667,6660,6669 or 7000 for a standard non secure connexion" << std::endl;
	std::cout << "6697 or 7001 for a SSL/TLS secure connexion" << std::endl;
	return(-1);
}

