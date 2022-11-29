/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcalon <jcalon@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/21 12:22:34 by mbascuna          #+#    #+#             */
/*   Updated: 2022/11/29 16:48:26 by jcalon           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/utils.hpp"

Server::Server(void) {
	this->_ip_address = "0.0.0.0";
	this->_index = "index.html";
	this->_listen.push_back("80");
	this->_autoindex = false;
	init_error_pages();
	init_allow_methods();
}

Server::~Server(void) {}

void Server::init_error_pages(void)
{
	this->_error_pages.insert(std::make_pair(400, "www/error_pages/400.html"));
	this->_error_pages.insert(std::make_pair(403, "www/error_pages/403.html"));
	this->_error_pages.insert(std::make_pair(404, "www/error_pages/404.html"));
	this->_error_pages.insert(std::make_pair(405, "www/error_pages/405.html"));
	this->_error_pages.insert(std::make_pair(413, "www/error_pages/413.html"));
	this->_error_pages.insert(std::make_pair(500, "www/error_pages/500.html"));
	this->_error_pages.insert(std::make_pair(502, "www/error_pages/502.html"));
}

void Server::init_allow_methods(void)
{
	this->_allow_method.push_back("GET");
	this->_allow_method.push_back("POST");
	this->_allow_method.push_back("DELETE");
	this->_allow_method.push_back("HEAD");
}

//surement appeler des fonctions pour chaque item . fonctions qui checkeront les cas d'erreur et syntax pour cahcun
std::vector<std::string>::iterator Server::parse_server(std::vector<std::string>::iterator start, std::vector<std::string> file)
{
	start++;
	for (;start != file.end(); start++)
	{
		std::vector<std::string> line;
		line = ft_cpp_split(*start, WHITESPACES);
		if (line[0] == "listen")
		{
			this->_listen.pop_back();
			for (size_t i = 1; i < line.size(); i++)
			{
				if (line[i].find(":") != std::string::npos)
				{
					std::vector<std::string> tmp = ft_cpp_split(line[i], ":");
					this->_ip_address = tmp[0];
					this->_listen.push_back(tmp[1]);
				}
				else
					this->_listen.push_back(line[i]);
			}
		}
		else if (line[0] == "server_name")
			this->_name.push_back(line[1]);
		else if (line[0] == "allow_method")
		{
			this->_allow_method.clear();
			for (size_t i = 0; i < line.size(); i++)
				this->_allow_method.push_back(line[i]);
		}
		else if (line[0] == "max_client_body_size")
			this->_max_client_body_size = line[1];
		else if (line[0] == "root")
			this->_root = line[1];
		else if (line[0] == "error_page" && line.size() > 2)
		{
			std::map<int, std::string>::iterator it = this->_error_pages.find(std::atoi(line[1].c_str()));
			if (it != _error_pages.end())
				it->second = line[2];
			else
				this->_error_pages.insert(std::make_pair(std::atoi(line[1].c_str()), line[2]));
		}
		else if (line[0] == "cgi_ext" && line.size() > 2)
			this->_cgi_ext.insert(std::make_pair(line[1], line[2]));
		else if (line[0] == "cgi_dir")
			this->_cgi_dir = line[1];
		else if (line[0] == "index")
			this->_index = line[1];
		else if (line[0] == "location")
		{
			Location location;
			if (line[line.size() - 1] != "{")
			{
				std::cout << "ERROR : location doit ouvrir avec {\n";
				break;
			}
			start = location.parse_location(start, file);
			this->_location.push_back(location);
			std::string verif = *start;
			if (verif.find("}") < 0 )
			{
				std::cout << "ERROR : location doit se fermer avec }" << std::endl;
				break ;
			}
		}
		else if (line[0] == "autoindex")
			(line[1] == "on" )? this->_autoindex = true : this->_autoindex = false;
		else
			break ;
	}
	return start;
}

std::vector<std::string> 			Server::get_listen(void) const { return this->_listen; }
std::string 						Server::get_ip(void) const { return this->_ip_address; }
std::string 						Server::get_index(void) const { return this->_index; }
std::vector<std::string> 			Server::get_name(void) const { return this->_name; }
std::string 						Server::get_root(void) const { return this->_root; }
std::map<int, std::string> 			Server::get_error_pages(void) const { return this->_error_pages; }
std::string 						Server::get_body_size(void) const { return this->_max_client_body_size;}
std::string 						Server::get_cgi_dir(void) const { return this->_cgi_dir;}
std::map<std::string, std::string> 	Server::get_cgi_ext(void) const { return this->_cgi_ext; }
bool 								Server::get_autoindex(void) const { return this->_autoindex; }
std::vector<Location> 				Server::get_location(void) const { return this->_location; }
std::vector<std::string> 			Server::get_allow_method(void) const { return this->_allow_method; }


std::ostream	&operator<<(std::ostream &o, Server const &Server) {
	o << BLUE << BOLD << " Server :" << RESET << std::endl;
	if (Server.get_listen().size())
	{
		o << "    listen = [";
		std::vector<std::string> listen = Server.get_listen();
		for (size_t i = 0; i < listen.size(); i++)
			o << listen[i] << " | ";
		o << "]" << std::endl;
	}
	if (Server.get_ip().size())
		o << "    ip = [" << Server.get_ip() << "]" << std::endl;
	if (Server.get_name().size())
	{
		o << "    Server_name = [";
		std::vector<std::string> name = Server.get_name();
		for (size_t i = 0; i < name.size(); i++)
			o << name[i] << " | ";
		o << "]" << std::endl;
}
		if (Server.get_allow_method().size() > 1)
	{
		o << "     allow method = [";
		std::vector<std::string> method = Server.get_allow_method();
		for (size_t i = 1; i < method.size(); i++)
			o << method[i] << " | ";
		o << "]" << std::endl;
	}
	if (Server.get_root().size())
		o << "    root = [" << Server.get_root() << "]" << std::endl;
	if (Server.get_error_pages().size())
	{
		o << "    error_page = [";
		std::map<int, std::string> error_pages = Server.get_error_pages();
		std::map<int, std::string>::iterator it = error_pages.begin();
		for (; it != error_pages.end(); it++)
			o << it->first << " , (" << it->second << ") | ";
		o << "]" << std::endl;
	}
	if (Server.get_body_size().size())
		o << "    body size = [" << Server.get_body_size() << "]" << std::endl;
	if (Server.get_cgi_dir().size() )
		o << "    cgi dir = [" << Server.get_cgi_dir() << "]" << std::endl;
	if (Server.get_cgi_ext().size())
		o << "    cgi ext = [" << Server.get_cgi_ext().at(0) << "]" << std::endl;
	if (Server.get_autoindex())
		o << "	autoindex" << Server.get_autoindex() << std::endl;
	if (Server.get_index().size())
		o << "    index = [" << Server.get_index() << "]" << std::endl;
	if (Server.get_location().size())
	{
		std::vector<Location> loc = Server.get_location();
		for (size_t i = 0; i < loc.size(); i++)
			o << loc[i];
	}
	return (o);
};
