#ifndef LIB_HPP
#define LIB_HPP

#include "Client.hpp"
#include <string>
#include <vector>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <poll.h>



int parserPort(char *portInString);
class server 
{
	typedef void (server::*CommandFunc)(int clientFd, std::string cmd);

	private :
		std::string _port;
		std::string _pass;
		std::vector<pollfd> _pfds;
		int _server_fd;
		int _new_socket;
		struct sockaddr_in _address;
		socklen_t _addrlen;	
		char _buffer[1024];

		std::map<int, Client> _clients;
		std::map<std::string, CommandFunc> _commands;
		
	public :
		server(std::string port, std::string password);
		~server();
		//todo
		std::string getPort(void);
		std::string getPass(void);
		void listenClient();
		void bindSocket();
		void acceptClient();
		void readInSocket();
		void sendMessage(std::string message);
		void sendWelcomeMessage(int client_fd, std::string nick);
 		void commandHandler(int clientFd, std::string cmd);

		void nick(int, std::string);


};

#endif