#include "../includes/server.hpp"
#include <iostream>
#include <cstring>
#include <sys/socket.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <cstdlib>
#include <poll.h>
#include <stdio.h>
#define PORT 8080
#define BACKLOG 5 // nombre de connexion simultanee a listen
// void start_server() 
// {
//     int server_fd, new_socket;
//     struct sockaddr_in address;
//     socklen_t addrlen = sizeof(address);
//     char buffer[1024] = {0};
//
//     server_fd = socket(AF_INET, SOCK_STREAM, 0);
//     if (server_fd == 0) 
// 	{
//         perror("socket failed");
//         exit(EXIT_FAILURE);
//     }
//
//     address.sin_family = AF_INET;
//     address.sin_addr.s_addr = INADDR_ANY;
//     address.sin_port = htons(PORT);
//
//     if (bind(server_fd, (struct sockaddr*)&address, sizeof(address)) < 0) {
//         perror("bind failed");
//         exit(EXIT_FAILURE);
//     }
//
//     if (listen(server_fd, BACKLOG) < 0) {
//         perror("listen failed");
//         exit(EXIT_FAILURE);
//     }
//     std::cout << "Server listening on port " << PORT << std::endl;
//
//     new_socket = accept(server_fd, (struct sockaddr*)&address, &addrlen);
//     if (new_socket < 0) {
//         perror("accept failed");
//         exit(EXIT_FAILURE);
//     }
//     std::cout << "Connection accepted!" << std::endl;
//
//     read(new_socket, buffer, 1024);
//     std::cout << "Received: " << buffer << std::endl;
//
//     std::string message = "Hello from server";
//     send(new_socket, message.c_str(), message.length(), 0);
//
//     close(new_socket);
//     close(server_fd);
// }
//
// void start_client() 
// {
//     int sock = 0;
//     struct sockaddr_in serv_addr;
//     char buffer[1024] = {0};
//
//     sock = socket(AF_INET, SOCK_STREAM, 0);
//     if (sock < 0) {
//         perror("Socket creation error");
//         exit(EXIT_FAILURE);
//     }
//
//     serv_addr.sin_family = AF_INET;
//     serv_addr.sin_port = htons(PORT);
//     if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0) {
//         perror("Invalid address");
//         exit(EXIT_FAILURE);
//     }
//
//     if (connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0) {
//         perror("Connection failed");
//         exit(EXIT_FAILURE);
//     }
//     std::cout << "Connected to server!" << std::endl;
//
//     std::string message = "Hello from client";
//     send(sock, message.c_str(), message.length(), 0);
//
//     read(sock, buffer, 1024);
//     std::cout << "Received from server: " << buffer << std::endl;
//
//     close(sock);
// }
//
// int main(int argc, char *argv[]) {
//     if (argc < 2) {
//         std::cout << "Usage: " << argv[0] << " <server|client>" << std::endl;
//         return 1;
//     }
//     
//     std::string mode = argv[1];
//     if (mode == "server") {
//         start_server();
//     } else if (mode == "client") {
//         start_client();
//     } else {
//         std::cout << "Invalid option. Use 'server' or 'client'" << std::endl;
//         return 1;
//     }
//     
//     return 0;
// }

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
	else
	{
		// start_server(); // modify the server to accept different port
			server server(argv[1],argv[2]);		
	}
	return(0);
}
