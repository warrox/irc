#include "../includes/Server.hpp"
#include <iostream>
#include <cstdlib>
#include <string>
#include <string>
#include <cstdlib>

void assertPortIsValid(const std::string &maybePort) {
	char* end;
	long port = std::strtol(maybePort.c_str(), &end, 10);
	// Check if the entire string was consumed and the value is in range
	if (*end != '\0' || port < 1 || port > 65535) {
		std::cerr << "Invalid port number: " << port << ": Must be in range 1-65535" << std::endl;
		exit(EXIT_FAILURE);
	}
}

int main(int argc, char **argv)
{
	if (argc < 3) {
		std::cout << "./ircserv <port> <password>" << std::endl;
		exit(EXIT_FAILURE);
	}

	std::string port(argv[1]);
	std::string password(argv[2]);

	assertPortIsValid(port);
	Server server(port, password);
	server.start();
	server.run();
	/*}*/
	return(0);
}
