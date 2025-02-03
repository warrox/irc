#ifndef LIB_HPP
#define LIB_HPP
#include <cctype>
#include <cstdio>
#include <cstring>
#include <iostream>
#include <sstream>
#include <poll.h>
#include <vector>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <functional>

int parserPort(char *portInString);
class server 
{
	private :
		std::string _port;
		std::string _pass;
		std::vector<pollfd> _pfds;
		struct sockaddr_in _address;
	public :
		server(std::string port, std::string password);
		~server();
		//todo
		std::string getPort(void);
		std::string getPass(void);
		void listenClient();
		void bindSocket();
		void accept();
		void read();
		void send();
};
#endif // !LIB_HPP
