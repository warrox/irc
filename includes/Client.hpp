/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cyferrei <cyferrei@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/06 14:54:19 by cyferrei          #+#    #+#             */
/*   Updated: 2025/02/06 16:45:49 by cyferrei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENT_HPP
#define CLIENT_HPP

#define CMD_NICK 1
#define CMD_USER 2

#include <string>
#include <map>

class Client {
	private:
		std::string _nick;
		std::string _user;
		std::string _host;
		std::string _real_name;
	public:
		Client() {};
		Client(int client_fd, std::string msgFromClient);
		~Client();

		void setNick(std::string nick);
		std::string getNick(void)const;

		void setUser(int client_fd, std::string msgFromClient, std::map<int, Client> &clients);
};

#endif