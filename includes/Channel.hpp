/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cyferrei <cyferrei@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/10 11:44:14 by whamdi            #+#    #+#             */
/*   Updated: 2025/02/17 15:42:40 by cyferrei         ###   ########.fr       */
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
		
		bool _inviteOnly;
		bool _topicRestricted;
		std::string _key;
		std::time_t _creationTime;
		std::string _modesChannel;
		
	public :

		Channel() : _chan_name(""), _server(NULL) {}
		Channel(std::string name, int fd, Server &);
		bool		addUser(int fd);
		bool 		delUser(int fd);
		void 		setTopic(std::string topic);
		std::string		getChanName(void);
		std::string getTopic(void);
		void		broadcast(int, Server&, std::string);	
		void printClientInChan(std::vector<int> user);
		std::vector<Client> getUsers(void);

		std::string getCreationTime()const;
		std::string getModeChan()const;
		bool isOperator(int);
		bool hasClient(std::string);
		void setModeI(bool);
		void setModeT(bool);
		void setModeK(bool, std::string key);
		void setOperator(int, bool);
		// stock le message du client dans un buffer > puis le renvoie a tous les clients non emeteur du message d'origine (e)
		~Channel() {}
	};
	
#endif
