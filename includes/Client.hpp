/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cyferrei <cyferrei@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/06 14:54:19 by cyferrei          #+#    #+#             */
/*   Updated: 2025/02/11 16:39:29 by cyferrei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <string>

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
		~Client() {};


		void	setNick(std::string nick);
		void	setUser(std::string user);
		void	setHost(std::string host);
		void	setRealName(std::string realname);
		
		std::string getNick(void)const;
		std::string getUser(void)const;
		std::string getHost(void)const;
		std::string getRealName(void)const;
		
		std::string	getChan(void);
};

#endif
