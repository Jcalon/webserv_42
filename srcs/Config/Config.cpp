/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Config.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbascuna <mbascuna@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/21 12:20:36 by mbascuna          #+#    #+#             */
/*   Updated: 2022/11/21 17:45:13 by mbascuna         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/Config/Config.hpp"
#include "../../includes/Config/Server.hpp"
#include <string>
#define WHITESPACES " \t;"

Config::Config(void) {}

Config::Config(std::vector<std::string> file) {
	parse_config(file);
}

Config::~Config(void) {}


void	Config::parse_config(std::vector<std::string> file)
{
	//parse workers
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
		}
		else
			break;
	}
}

std::string			Config::get_workers(void) const { return this->_workers; }
std::vector<Server>	Config::get_server(void) const { return this->_server; }
std::string			Config::get_max_connections(void) const { return this->_max_connections; }

std::vector<std::string> ft_cpp_split(const std::string &str, const std::string &charset)
{
	std::vector<std::string> res;
	std::string			tmp;
	size_t			i;

	i = 0;
	while (i < str.length())
	{
		while (i < str.length() && ft_in_charset(str[i], charset))
			i++;
		if (i < str.length())
		{
			tmp = "";
			while (i < str.length() && !ft_in_charset(str[i], charset))
				tmp += str[i++];
			res.push_back(tmp);
		}
	}
	return res;
}

bool	ft_in_charset(char const c, const std::string &charset)
{
	int	i_charset;

	i_charset = 0;
	while (charset[i_charset])
	{
		if (c == charset[i_charset++])
			return true;
	}
	return false;
}

std::ostream &operator<<(std::ostream &o, Config const &rhs)
{
	o << "workers : " << rhs.get_workers() << std::endl;
	o << "max_connections : " << rhs.get_max_connections() << std::endl;
	o << "servers : " << std::endl;
	std::vector<Server> srv = rhs.get_server();
	size_t i = 0;
	while (i < srv.size())
	{
		o << srv[i];
		i++;
	}
	return o;
}
