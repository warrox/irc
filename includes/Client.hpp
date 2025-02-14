/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cyferrei <cyferrei@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/06 14:54:19 by cyferrei          #+#    #+#             */
/*   Updated: 2025/02/14 16:10:22 by whamdi           ###   ########.fr       */
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
		std::string _password;
		
		bool mode_i;
		bool mod_t;
		bool mode_k;
		bool mode_o;
		bool _is_connected;
		
		std::string _actual_chan;
	public:
		Client();
		Client(int client_fd, std::string msgFromClient);
		// Client& operator=(const Client &);
		~Client() {};


		void	setNick(std::string nick);
		void	setUser(std::string user);
		void	setHost(std::string host);
		void	setRealName(std::string realname);
		void	setPassword(std::string);
		void	setIsConnected(bool);
		void	setModeI(bool);
		void	setModeT(bool);
		void	setModeK(bool);
		void	setModeO(bool);


		
		std::string getNick(void)const;
		std::string getUser(void)const;
		std::string getHost(void)const;
		std::string getRealName(void)const;
		std::string getPassword(void)const;
		bool	getIsConnected(void)const;		
		std::string	getChan(void); // ? const ?
		bool getModeI(void)const;
		bool getModeT(void)const;
		bool getModeK(void)const;
		bool getModeO(void)const;

};

#endif
