/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbascuna <mbascuna@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/21 12:22:34 by mbascuna          #+#    #+#             */
/*   Updated: 2022/11/21 17:56:23 by mbascuna         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/Config/Server.hpp"
#include "../../includes/Config/Config.hpp"
#define WHITESPACES " \t;"


Server::Server(void) {}

Server::~Server(void) {}


//surement appeler des fonctions pour chaque item . fonctions qui checkeront les cas d'erreur et syntax pour cahcun
std::vector<std::string>::iterator Server::parse_server(std::vector<std::string>::iterator start, std::vector<std::string> file)
{
	start++;
	for (;start != file.end(); ++start)
	{
		std::vector<std::string> line;
		line = ft_cpp_split(*start, WHITESPACES);
		if (line[0] == "listen")
			this->_listen.push_back(line[1]);
		else if (line[0] == "server_name")
			this->_name.push_back(line[1]);
		else if (line[0] == "max_client_body_size")
			this->_max_client_body_size = line[1];
		else if (line[0] == "root")
			this->_root = line[1];
		else if (line[0] == "error_page" && line.size() > 2)
			this->_error_pages.insert(std::make_pair(line[1], line[2]));
		else if (line[0] == "cgi_ext" && line.size() > 2)
			this->_cgi_ext.insert(std::make_pair(line[1], line[2]));
		else if (line[0] == "cgi_dir")
			this->_cgi_dir = line[1];
		else if (line[0] == "location")
		{
			if (line[line.size() - 1] != "{")
			{
				std::cout << "ERROR : location doit ouvrir avec {\n";
				break;
			}
			Location location;
			start = location.parse_location(start, file);
			this->_location.push_back(location);
			if (*start != "  }")
			{
				std::cout << "ERROR : location doit se fermer avec }" << std::endl;
				break;
			}
		}
		else if (line[0] == "autoindex")
			(line[1] == "on" )? this->_autoindex = true : this->_autoindex = false;
		else
			break;
	}
	return start;
}

std::vector<std::string> Server::get_listen(void) const { return this->_listen; }
std::vector<std::string> Server::get_name(void) const { return this->_name; }
std::string Server::get_root(void) const { return this->_root; }
std::map<std::string, std::string> Server::get_error_pages(void) const { return this->_error_pages; }
std::string Server::get_body_size(void) const { return this->_max_client_body_size;}
std::string Server::get_cgi_dir(void) const { return this->_cgi_dir;}
std::map<std::string, std::string> Server::get_cgi_ext(void) const { return this->_cgi_ext; }
bool Server::get_autoindex(void) const { return this->_autoindex; }
std::vector<Location> Server::get_location(void) const { return this->_location; }



std::ostream	&operator<<(std::ostream &o, Server const &server) {
	o << "server :" << std::endl;
	if (server.get_listen().size())
		o << "    listen = [" << server.get_listen().front() << "]" << std::endl;
	if (server.get_name().size())
		o << "    server_name = [" << server.get_name().front() << "]" << std::endl;
	if (server.get_root().size())
		o << "    root = [" << server.get_root() << "]" << std::endl;
	if (server.get_error_pages().size())
		o << "    error_page = [" << server.get_error_pages().size() << "]" << std::endl;
	if (server.get_body_size().size() )
		o << "    body size = [" << server.get_body_size() << "]" << std::endl;
	if (server.get_cgi_dir().size() )
		o << "    cgi dir = [" << server.get_cgi_dir() << "]" << std::endl;
	if (server.get_cgi_ext().size())
		o << "    cgi ext = [\n" << server.get_cgi_ext().at(0) << "]" << std::endl;
	if (server.get_autoindex())
		o << "	autoindex" << server.get_autoindex() << std::endl;
	if (server.get_location().size())
	{
		std::vector<Location> loc = server.get_location();
		size_t i = 0;
		while (i < loc.size())
		{
			o << "       location\n ";
			o << loc[i];
			i++;
		}
	}
	return (o);
};
