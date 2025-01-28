#include <cctype>
#include <cstdio>
#include <cstring>
#include <iostream>
#include <sstream>
int parserPort(char *portInString)
{
	for(int i = 0; i < strlen(portInString);i++)
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
int main(int argc, char **argv)
{
	if(argc < 3)
	{
		std::cout << "./ircserv <port> <password>" << std::endl;
		return(1);	
	}
	int port = 	parserPort(argv[1]);
	if(port == -1)
		return(1);
	return(0);
}
