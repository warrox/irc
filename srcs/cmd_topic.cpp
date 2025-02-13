/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_topic.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cyferrei <cyferrei@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/13 10:44:28 by cyferrei          #+#    #+#             */
/*   Updated: 2025/02/13 10:45:03 by cyferrei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Server.hpp"

#include <sstream>
#include <iostream>

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