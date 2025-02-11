/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cyferrei <cyferrei@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/06 14:54:19 by cyferrei          #+#    #+#             */
/*   Updated: 2025/02/11 11:13:16 by whamdi           ###   ########.fr       */
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
		std::string _actual_chan;
	public:
		Client() {};
		Client(int client_fd, std::string msgFromClient);
		//Same as in Server.hpp
		~Client() {};

		//Tabulation for readability
		/*void setNick(std::string nick);*/
		void		setNick(std::string nick);
		std::string getNick(void)const;
		void		getChan(void);

		//This is bullshit
		//This function wasn't yet defined and we're all glad it never been, thanks god
		/*void setUser(int client_fd, std::string msgFromClient, std::map<int, Client> &clients);*/
};

#endif
