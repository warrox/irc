/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cyferrei <cyferrei@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/14 12:07:03 by whamdi            #+#    #+#             */
/*   Updated: 2025/02/20 13:56:36 by cyferrei         ###   ########.fr       */
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
		std::map<int,Client> getClients()const;
		//--------------------- LOG / DEBUG --------------------//
		void sendAndLog(int, std::string);
		void log(std::string);
		void fatal(std::string);
		void recvLog(int clientFd, std::string message);
		void displayClientsInfo(void);
		void sendMessageto(int clientFd, int receiver_fd, std::string msg, std::string sender_name, std::string receiver_name);	
		void sendingUserListToClient(std::string chanName,int clientFd, bool);
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
		void kick(int clientFd, std::string cmd);
		Client getClient(int client_fd) { return _clients[client_fd]; }
		
		int findClientByName(std::string name, std::map<int,Client> client);
		//-------------------- GRAMMATICAL LANGUAGE--------------- //
		std::string get_prefix(int);
		//-------------------- MODE--------------- //
		void case_mode_channel(std::string, std::string, std::string, int);
		void case_mode_user(std::string, std::string, int);
		bool is_user(std::string);
		bool is_channel(std::string);

		void mode_i(int clientFd, bool addMode, std::string target, std::string mode, std::map<std::string, Channel>::iterator match);
		void mode_o(int clientFd, std::string target, std::string target_user, bool addMode, std::string mode, std::map<std::string, Channel>::iterator match);
		void mode_t(int clientFd, bool addMode, std::string target, std::string mode, std::map<std::string, Channel>::iterator match);
		void mode_l(int clientFd, bool addMode, std::string target, std::string target_user, std::string mode, std::map<std::string, Channel>::iterator match);
		void mode_k(int clientFd, bool addMode, std::string target, std::string target_user, std::string mode, std::map<std::string, Channel>::iterator match);

};

#endif
