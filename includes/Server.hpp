/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: whamdi <whamdi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/14 12:07:03 by whamdi            #+#    #+#             */
/*   Updated: 2025/02/15 12:29:58 by whamdi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LIB_HPP
#define LIB_HPP

#include "Client.hpp"
#include "Channel.hpp"
#include <map>
#include <string>
#include <vector>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <poll.h>

//Avoid magic numbers by defining whats static
#define BUFFER_SIZE 1024

int parserPort(char *portInString);

class Server 
{
	typedef void (Server::*CommandFunc)(int clientFd, std::string cmd);
	typedef std::map<std::string, Channel>::iterator channelIterator;
	typedef std::map<int , Client>::iterator clientIterator;
	typedef std::pair<std::string, Channel> channelEntry;
 
	private :
		std::string _port;
		std::string _pass; //You might consider hashing the password, storing a plain text password is bullshit
		std::string _servername;
		std::string _time;

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

		std::string getPass(void);
		void start();
		void run();
		void acceptNewClient();
		void scanClients();
 		void commandHandler(int clientFd, std::string cmd);
		void disconnectClient(int clientFd, const std::string& reason);
		//--------------------- LOG / DEBUG --------------------//
		void sendAndLog(int, std::string);
		void log(std::string);
		void fatal(std::string);
		void recvLog(int clientFd, std::string message);
		void displayClientsInfo(void);

		//-------------------- SERVER COMMANDS --------------- //
		void nick(int, std::string);
		void setNewNick(int, std::string);
		bool isNickTaken(std::string);
		void pass(int, std::string);
		void user(int, std::string);
		void mode(int, std::string);
		void topic(int, std::string);
		void privmsg(int, std::string);
		void quit(int, std::string);
		void ping(int, std::string);	
		void join(int clientFd, std::string cmd);
		void sendMessageto(int clientFd, int receiver_fd, std::string msg, std::string sender_name, std::string receiver_name);

		Client getClient(int client_fd) { return _clients[client_fd]; }
		
		int findClientByName(std::string name, std::map<int,Client> client);
		//-------------------- GRAMMATICAL LANGUAGE--------------- //
		std::string get_prefix(int);
};

#endif
