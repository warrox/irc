/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_topic.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cyferrei <cyferrei@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/13 10:44:28 by cyferrei          #+#    #+#             */
/*   Updated: 2025/02/17 15:12:44 by whamdi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Server.hpp"

#include <sstream>
#include <iostream>

void Server::topic(int clientFd, std::string cmd)
{
    std::istringstream line(cmd);
    std::string commandName;
    std::string chan;
    std::string topic_name;

    line >> commandName;
    line >> chan;
    std::getline(line, topic_name);

    if (!topic_name.empty() && topic_name[1] == ':')
        topic_name.erase(0, 2);

    std::string actual_chan = this->_clients[clientFd].getChan();
    std::cout << "cmd brut : " << cmd << std::endl;

    if (actual_chan != "NO")
    {
        this->_channels[chan].setTopic(topic_name);

        std::string response = ":" + this->_servername + " TOPIC " + chan + " :" + topic_name + "\r\n";


        send(clientFd, response.c_str(), response.size(), 0);
    }
    else
    {
        std::string errorMsg = ":" + this->_servername + " 442 " + this->_clients[clientFd].getNick() + " " + chan + " :You're not on that channel\r\n";
        send(clientFd, errorMsg.c_str(), errorMsg.size(), 0);
    }
}
