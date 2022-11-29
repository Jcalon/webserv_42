/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Config.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcalon <jcalon@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/21 12:20:36 by mbascuna          #+#    #+#             */
/*   Updated: 2022/11/23 12:53:22 by jcalon           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/utils.hpp"

Config::Config(void) {}

Config::Config(char *argv)
{
	std::ifstream   ifs(argv);
	std::vector<std::string> file;
	if (ifs.is_open())
	{
		std::string	line;
		while (std::getline(ifs, line, '\n'))
		{
			if (line.size())
				file.push_back(line);
		}
	}
	else
		throw Config::FileNotOpen();
	ifs.close();
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
			std::vector<std::string> listen = server.get_listen();
			std::vector<Server>::iterator it_check = check_server(listen);
			if (it_check == _server.end())
			{
				this->_server.push_back(server);
			}
			else if ((it_check != _server.end()) && listen[listen.size() - 1] == "default")
			{
				this->_server.erase(it_check);
				this->_server.push_back(server);
			}
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

std::vector<Server>::iterator Config::check_server(std::vector<std::string> listen)
{
	std::vector<Server>::iterator it = _server.begin();
	size_t i = 0;
	for (; it != _server.end(); it++)
	{
		std::vector<std::string> to_check = _server[i].get_listen();
		for (size_t j = 0; j < to_check.size(); j++)
		{
			if (listen[j] == to_check[j])
			{
				return it;
			}
		}
		i++;
	}
	return it;
}


const char *Config::FileNotOpen::what() const throw()
{
	return ("Error opening infile!");
}




std::ostream &operator<<(std::ostream &o, Config const &rhs)
{
	std::cout << std::endl;
	std::cout << RED << BOLD << TAB << "*********** Config ***********" << RESET << std::endl;
	o << "workers : " << rhs.get_workers() << std::endl;
	o << "max_connections : " << rhs.get_max_connections() << std::endl;
	std::vector<Server> srv = rhs.get_server();
	for (size_t i = 0; i < srv.size(); i++)
		o << srv[i];
	std::cout << std::endl;
	std::cout << RED << BOLD << TAB << "************ end *************" << RESET << std::endl;

	return o;
}
