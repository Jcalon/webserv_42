/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Config.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbascuna <mbascuna@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/21 12:20:36 by mbascuna          #+#    #+#             */
/*   Updated: 2022/11/22 11:34:30 by mbascuna         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/utils.hpp"

Config::Config(void) {}

Config::Config(std::vector<std::string> file)
{
	parse_config(file);
}

Config::~Config(void) {}


void	Config::parse_config(std::vector<std::string> file)
{
	std::vector<std::string> line;
	for (std::vector<std::string>::iterator it = file.begin(); it != file.end(); it++)
	{
		line = ft_cpp_split(*it, WHITESPACES);
		if (line[0] == "workers")
			this->_workers = line[1];
		else if (line[0] == "max_connections")
			this->_max_connections = line[1];
		else if (line[0] == "server")
		{
			Server server;
			if (line[1] != "{")
			{
				std::cout << " ERROR \n";
				return ;
			}
			it = server.parse_server(it, file);
			this->_server.push_back(server);
			std::string verif = *it;
			if (verif.find("}") < 0)
			{
				std::cout << "ERROR : server  doit se fermer avec }" << std::endl;
				break;
			}
		}
		else
			break;
	}
}

std::string			Config::get_workers(void) const { return this->_workers; }
std::vector<Server>	Config::get_server(void) const { return this->_server; }
std::string			Config::get_max_connections(void) const { return this->_max_connections; }


std::ostream &operator<<(std::ostream &o, Config const &rhs)
{
	std::cout << std::endl;
	std::cout << RED << BOLD << "********* Config *********" << RESET << std::endl;
	o << "workers : " << rhs.get_workers() << std::endl;
	o << "max_connections : " << rhs.get_max_connections() << std::endl;
	std::vector<Server> srv = rhs.get_server();
	size_t i = 0;
	while (i < srv.size())
	{
		o << srv[i];
		i++;
	}
	return o;
}
