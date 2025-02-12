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

void Server::disconnectClient(int clientFd, const std::string& reason) {
    if (send(clientFd, "", 0, MSG_NOSIGNAL) != -1) {
        std::string errorMsg = ":localhost ERROR :Closing Link: " + reason + "\r\n";
        sendAndLog(clientFd, errorMsg);
    }

    close(clientFd);
    _clients.erase(clientFd);
    std::cout << RED << "Client " << clientFd << " disconnected: " << reason << RESET << std::endl;
}
void Server::user(int clientFd, std::string cmd) 
{
    std::string pass = this->_clients[clientFd].getPassword();	
    if (pass.empty() || this->_pass != pass) {
        std::cout << RED << "Wrong Password connection denied" << RESET << std::endl;
        std::string errorMsg = ":localhost 464 * :Incorrect password\r\n";
        this->sendAndLog(clientFd, errorMsg);

        disconnectClient(clientFd, "Incorrect password");
        return;
    }

    std::istringstream iss(cmd);
    std::string command;
    std::string nick;
    std::string user;
    std::string host;
    std::string realname;

    iss >> command >> nick >> user >> host;
    std::getline(iss, realname);

    if (user.empty() || host.empty() || realname.empty() || realname[1] != ':') {
        sendAndLog(clientFd, ":localhost 461 " + _clients[clientFd].getNick() + " USER :\r\n");
        disconnectClient(clientFd, "Invalid USER command");
        return;
    }

    _clients[clientFd].setUser(user);
    _clients[clientFd].setHost(host);
    _clients[clientFd].setRealName(realname.substr(2));
    this->displayClientsInfo();
}
bool Server::isNickTaken(std::string nickname){

	for (std::map<int, Client>::const_iterator it = _clients.begin(); it != _clients.end(); ++it) {
		if (it->second.getNick() == nickname)
			return true;
	}
	return false;
}

void Server::setNewNick(int clienFd, std::string nickname) {
	static int suf = 0;
	std::stringstream new_nickname;

	while (isNickTaken(new_nickname.str())) {
		new_nickname << nickname << suf;
		suf++;
	}
	_clients[clienFd].setNick(new_nickname.str());
}

void Server::nick(int clientFd, std::string cmd) {

	std::istringstream iss(cmd);
	std::string command;
	std::string nickname;

	iss >> command >> nickname;
	if (nickname.empty()) {
		sendAndLog(clientFd, ":localhost 461 " + _clients[clientFd].getNick() + " NICK :\r\n");
		return;
	}
	for (std::map<int, Client>::iterator match = _clients.begin(); match != _clients.end(); ++match) {
		if (match->second.getNick() == nickname) {
			sendAndLog(clientFd, ":localhost 433 " + _clients[clientFd].getNick() + " NICK :" + nickname + "\r\n");
			setNewNick(clientFd, nickname);
			return;
		}
	}
	_clients[clientFd].setNick(nickname);
}
void Server::privmsg(int clientFd, std::string cmd)
{
	std::istringstream lineStream(cmd);
	std::string cmdName, dest, message;
	lineStream >> cmdName;
	lineStream >> dest;
	lineStream >> cmdName;
	std::getline(lineStream, message);



	if (dest.at(0) == '#') {
		channelIterator it = this->_channels.find(dest);
		if (it == this->_channels.end())
			return ;
		this->log("Broadcasting on channel: " + dest);
		it->second.broadcast(clientFd, *this, message);
	}
	else{
		//je cherche le fd du client destinataire
		//j'ai comme information, le 'nick' de ce client
		//comment l'obtenir ?
		clientIterator it = this->_clients.begin();
		for(; it != this->_clients.end(); ++it)
		{
			if(it->second.getNick() == dest) {
				this->sendAndLog(it->first, message);
			}
		}
	}
	(void)clientFd;
	// clientIterator it = _clients.begin();


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
		this->log("Channel created: " + chanName);
		it = this->_channels.find(chanName);
		it->second.addUser(clientFd);
		std::string log = "Added client to " + chanName;
		this->log(log);
		//Do you really think this is redeable ? maintainable ?
		/*this->_channels.insert(std::make_pair<chanName, Channel(chanName, clientFd));*/
	} else {
		//Add the client before logging the information, doesn't make sense othewise
		//If you have an exception in addUser, you'll never get where it came from cuz you
		//Asserted it was ok by logging before actually updating the value
		it->second.addUser(clientFd);
		std::string log = "Added client to " + chanName;
		this->log(log);
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
	this->_clients[clientFd].setPassword(password);
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
	if(!this->_clients[clientFd].getIsConnected())
	{

	}
}
