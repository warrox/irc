/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cyferrei <cyferrei@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/10 11:44:14 by whamdi            #+#    #+#             */
/*   Updated: 2025/02/13 10:51:08 by cyferrei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CHANNEL_HPP
#define CHANNEL_HPP

#include <vector>
#include <string>

class Server;

class Channel {

	private:
		std::string _chan_name;
		std::string _chan_format; // reprend le nom du chan et le format pour chaque user	
		std::vector<int> _users;
		std::string _topic;
	public :
		Channel(std::string name, int fd);

		bool		addUser(int fd);
		bool 		delUser(int fd);
		void 		setTopic(std::string topic);
	std::string		getChanName(void);
		std::string getTopic(void);
		void		broadcast(int, Server&, std::string);
		// stock le message du client dans un buffer > puis le renvoie a tous les clients non emeteur du message d'origine (e)
		~Channel() {}
	};
	
#endif