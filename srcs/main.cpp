#include "../includes/lib.hpp"


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
