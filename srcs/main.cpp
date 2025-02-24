/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cyferrei <cyferrei@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/13 10:48:07 by cyferrei          #+#    #+#             */
/*   Updated: 2025/02/24 16:25:50 by whamdi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Server.hpp"

#include <iostream>
#include <cstdlib>
#include <string>
#include <csignal>

Server* g_server = NULL; // Pointeur global pour accéder au serveur

void assertPortIsValid(const std::string &maybePort) {
	char* end;
	long port = std::strtol(maybePort.c_str(), &end, 10);
	if (*end != '\0' || port < 1 || port > 65535) {
		std::cerr << "Invalid port number: " << port << ": Must be in range 1-65535" << std::endl;
		exit(EXIT_FAILURE);
	}
}

void handle_signal(int signal) {
	(void)signal;
	if (g_server) {
		std::cout << "[SERVER] SIGINT received, closing the IRC server" << std::endl;
		delete g_server;
		g_server = NULL;
	}
	std::exit(0);
}

int main(int argc, char **argv) {
	signal(SIGINT, handle_signal); // Capture CTRL+C

	if (argc < 3) {
		std::cout << "./ircserv <port> <password>" << std::endl;
		exit(EXIT_FAILURE);
	}

	std::string port(argv[1]);
	std::string password(argv[2]);

	assertPortIsValid(port);

	g_server = new Server(port, password);
	g_server->start();
	g_server->run();

	delete g_server;
	g_server = NULL;
	
	return 0;
}
