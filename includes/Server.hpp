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


//Avoid magic numbers by defining whats static
#define BUFFER_SIZE 1024


int parserPort(char *portInString);
class Server 
{
	typedef void (Server::*CommandFunc)(int clientFd, std::string cmd);
	typedef std::map<std::string, Channel>::iterator channelIterator;
	typedef std::pair<std::string, Channel> channelEntry;
 
	private :
		std::string _port;
		std::string _pass; //You might consider hashing the password, storing a plain text password is bullshit

		// ----- Socket related ------ //
		std::vector<pollfd> _pfds;
		int _server_fd;
		int _new_socket;
		struct sockaddr_in _address;
		socklen_t _addrlen;	
		char _buffer[1024];

		//-------- IRC related database ---------- //
		std::map<std::string, Channel> _channels;
		std::map<int, Client> _clients;
		std::map<std::string, CommandFunc> _commands;
		
	public :
		Server(std::string port, std::string password);

		//You desctructor shouldn't be empty, but if it, you can initialise it here via syntax {}
		~Server() {}

		//Useless
		/*std::string getPort(void);*/

		std::string getPass(void);
		void start();
		void run();
		void acceptNewClient();
		void scanClients();
 		void commandHandler(int clientFd, std::string cmd);

		//--------------------- LOG / DEBUG --------------------//
		void sendAndLog(int, std::string);
		void log(std::string);
		void fatal(std::string);
		void recvLog(int clientFd, std::string message);

		void sendWelcomeMessage(int client_fd, std::string nick);
		//-------------------- SERVER COMMANDS --------------- //
		void nick(int, std::string);
		void pass(int, std::string);
		void join(int, std::string);
		void topic(int, std::string);
		//Why omitting the space, don't you like order ????
		//(right after the coma)
		/*void join(int,std::string);*/
};

#endif
