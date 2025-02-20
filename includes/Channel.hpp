/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cyferrei <cyferrei@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/10 11:44:14 by whamdi            #+#    #+#             */
/*   Updated: 2025/02/20 10:58:44 by whamdi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CHANNEL_HPP
#define CHANNEL_HPP

#include <vector>
#include <string>
#include <ctime>

#include "../includes/Client.hpp"

class Server;

class Channel {

	private:
		std::string _chan_name;
		std::string _chan_format; // reprend le nom du chan et le format pour chaque user		
		std::vector<int> _users;
		Server *_server;
		std::string _topic;

		bool _modeI;
		bool _modeT;
		std::vector<Client*> _usersInChannel;
		
	public :

		std::vector<std::string> client_invite_lst;
		Channel() : _chan_name(""), _server(NULL), _modeI(false) {}
		Channel(std::string name, int fd, Server &);
		bool		addUser(int fd);
		bool 		delUser(int fd);
		void 		setTopic(std::string topic);
		std::string		getChanName(void);
		std::string getTopic(void);
		void		broadcast(int, Server&, std::string, bool);	
		void printClientInChan(std::vector<int> user);
		std::vector<Client> getUsers(void);

		
		void setModeI(bool);
		void setModeT(bool);
		
		bool getModeI(void)const;
		bool getModeT(void)const;
		
		void addUserInChannel(Client &client);
		void removeUserInChannel(Client);
		bool isUserInChannel(std::string);
		void setModeOInChannel(std::string, bool);
		Client *getTarget(std::string);
				
		~Channel() {}
	};
	
#endif
