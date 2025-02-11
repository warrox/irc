#ifndef LIB_HPP
#define LIB_HPP

#include "Client.hpp"
#include <string>
#include <vector>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <poll.h>
#include "Channel.hpp"

#define PORT 8080
#define BACKLOG 5 // nombre de connexion simultanee a listen


int parserPort(char *portInString);
class server 
{
	typedef void (server::*CommandFunc)(int clientFd, std::string cmd);
	typedef std::map<std::string, Channel>::iterator channelIterator;
	typedef std::pair<std::string, Channel> channelEntry;
 
	private :
		std::string _port;
		std::string _pass;
		std::vector<pollfd> _pfds;
		int _server_fd;
		int _new_socket;
		struct sockaddr_in _address;
		socklen_t _addrlen;	
		char _buffer[1024];
		std::map<std::string, Channel> _channels;
		std::map<int, Client> _clients;
		std::map<std::string, CommandFunc> _commands;
		
	public :
		server(std::string port, std::string password);

		//You desctructor shouldn't be empty, but if it, you can initialise it here via syntax {}
		~server() {}

		//Useless
		/*std::string getPort(void);*/

		std::string getPass(void);
		void listenClient();
		void bindSocket();
		void acceptClient();
		void readInSocket();
		void sendMessage(std::string message);
		void sendWelcomeMessage(int client_fd, std::string nick);
 		void commandHandler(int clientFd, std::string cmd);

		//--------------------- LOG / DEBUG --------------------//
		void sendAndLog(int, std::string);
		void log(std::string);
		void fatal(std::string);

		//-------------------- SERVER COMMANDS --------------- //
		void nick(int, std::string);
		void pass(int, std::string);
		void join(int, std::string);
		//Why omitting the space, don't you like order ????
		//(right after the coma)
		/*void join(int,std::string);*/
};

#endif
