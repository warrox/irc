#include "../includes/Server.hpp"
#include <sstream>
#include <iostream>
#include <string>
#include <unistd.h>
#include "../includes/colors.hpp"

void Server::sendWelcomeMessage(int clientFd, std::string nick)
{
    std::string welcome = ":localhost 001 " + nick + " :Bienvenue sur mon serveur IRC !\r\n";
	this->sendAndLog(clientFd, welcome);
}

void Server::nick(int clientFd, std::string cmd) {
	(void)clientFd; (void)cmd;
}

void Server::join(int clientFd, std::string cmd)
{
	std::istringstream lineStream(cmd);

	//Declare two strings at once
	std::string cmdName, chanName;
	lineStream >> cmdName;
	lineStream >> chanName;

	//We made a typedef together for readability, use it
    channelIterator it = this->_channels.find(chanName);

	if(it == this->_channels.end()) {
		//Added a new typdef, which is a pair of std::string, Channel::Channel
		//We call every pair < key, value > in a map an Entry, see map definition on cpp reference
		//Improved readability.
		channelEntry newEntry(chanName, Channel(chanName, clientFd));
		this->_channels.insert(newEntry);	
		//Do you really think this is redeable ? maintainable ?
		/*this->_channels.insert(std::make_pair<chanName, Channel(chanName, clientFd));*/
	} else {
		//Add the client before logging the information, doesn't make sense othewise
		//If you have an exception in addUser, you'll never get where it came from cuz you
		//Asserted it was ok by logging before actually updating the value
		it->second.addUser(clientFd);
		std::string log = "Added client to " + chanName;
		this->log(chanName);
		/*it->second.addUser(clientFd);*/
	}
}

//Removed getline, no need to split lines, we did it in Server::commandHandler
void Server::pass(int clientFd, std::string cmd) {
	std::istringstream line(cmd);

	std::string commandName;
	std::string password;
	line >> commandName;
	line >> password;

	std::cout << "COMMAND: " << cmd << std::endl;

	//Handle error first
	if (password != this->_pass) {
		std::cout << RED << "Wrong Password connection denied" << RESET << std::endl;
		std::string errorMsg = ":localhost 464 * :Incorrect password\r\n";
		this->sendAndLog(clientFd, errorMsg);
		close(clientFd);
		return ;
	}

	//Log infos in the cout, see Server::log, Server::sendAndLog
	std::string message("Password ok");
	this->log(message);
}

void Server::topic(int clientFd, std::string cmd)
{
	std::istringstream line(cmd);
	std::string commandName;
	std::string topic_name;
	line >> commandName;
	line >> topic_name;
	std::string actual_chan =this->_clients[clientFd].getChan();
	if(actual_chan != "NO")
	{
		this->_channels.find(actual_chan)->second.setTopic(topic_name);	
		std::cout<< "Topic : " << this->_channels.find(actual_chan)->second.getTopic();
	}
}
void Server::commandHandler(int clientFd, std::string cmd) {
	std::istringstream stream(cmd);
	std::string line;

	while(std::getline(stream, line)) {
		std::istringstream lineStream(line);
		std::string commandName;
		lineStream >> commandName;

		std::map<std::string, CommandFunc>::iterator match = this->_commands.find(commandName);
		if (match != this->_commands.end()) {
			(this->*_commands[commandName])(clientFd, line);
		} else {
			std::stringstream log;
			log << commandName << ": Command not recognized";
			this->log(log.str());
		}
	}
	//Pretty useless return statement.
	/*return ;*/
}
