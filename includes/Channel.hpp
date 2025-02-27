/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cyferrei <cyferrei@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/10 11:44:14 by whamdi            #+#    #+#             */
/*   Updated: 2025/02/24 18:41:44 by cyferrei         ###   ########.fr       */
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
		bool _modeL;
		bool _modeK;
		int _limitValue;
		std::string _keyChannel;
		
		std::vector<std::string> _userNameList;
		
		public :
		
		Channel();
		Channel(std::string name, Server &);
		~Channel() {}
		
		std::vector<Client*> _usersInChannel;
		std::vector<std::string> client_invite_lst;
		bool		addUser(int fd);
		bool 		delUser(int fd);
		void 		setTopic(std::string topic);
		std::string		getChanName(void);
		std::string getTopic(void);
		void		broadcast(int clientToIgnore, Server &server, std::string msg, bool is_for_topic);	
		void printClientInChan(std::vector<int> user);
		std::vector<Client> getUsers(void);
		
		void setModeI(bool);
		void setModeT(bool);
		void setModeL(bool);
		void setLimitValue(int);
		void setModeK(bool);
		void setKeyChannel(std::string);
		
		bool getModeI(void)const;
		bool getModeT(void)const;
		bool getModeL(void)const;
		bool getModeK(void)const;
		int getLimitValue(void)const;
		std::string getKeyChannel(void)const;
		
		void addUserInChannel(Client &client);
		void addNameInListChannel(std::string);
		void removeUserInChannel(Client &client);
		void removeNameUserInChannel(std::string);
		bool isUserInChannel(std::string);
		bool isUserInvitedInChannel(std::string);
		void setModeOInChannel(std::string, bool);
		Client *getTarget(std::string);

		void displayChannelInfos();
		size_t getNbUsersInChannel();
	};
	
#endif
