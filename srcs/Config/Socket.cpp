/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Socket.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbascuna <mbascuna@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/21 12:22:34 by mbascuna          #+#    #+#             */
/*   Updated: 2022/11/22 16:03:50 by mbascuna         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/utils.hpp"

Socket::Socket(void) {
	this->_ip_address = "0.0.0.0";
	this->_listen.push_back(80);
	this->_autoindex = false;
}

Socket::~Socket(void) {}


//surement appeler des fonctions pour chaque item . fonctions qui checkeront les cas d'erreur et syntax pour cahcun
std::vector<std::string>::iterator Socket::parse_server(std::vector<std::string>::iterator start, std::vector<std::string> file)
{
	start++;
	for (;start != file.end(); ++start)
	{
		std::vector<std::string> line;
		line = ft_cpp_split(*start, WHITESPACES);
		if (line[0] == "listen")
		{
			//supprime valeur par default
			this->_listen.pop_back();
			for (size_t i = 1; i < line.size(); i++)
			{
				if (line[i].find(":") != std::string::npos)
				{
					std::vector<std::string> tmp = ft_cpp_split(line[i], ":");
					this->_ip_address = tmp[0];
					this->_listen.push_back(std::atoi(tmp[1].c_str()));
				}
				else
					this->_listen.push_back(std::atoi(line[i].c_str()));
			}
		}
		else if (line[0] == "server_name")
			this->_name.push_back(line[1]);
		else if (line[0] == "max_client_body_size")
			this->_max_client_body_size = line[1];
		else if (line[0] == "root")
			this->_root = line[1];
		else if (line[0] == "error_page" && line.size() > 2)
			this->_error_pages.insert(std::make_pair(std::atoi(line[1].c_str()), line[2]));
		else if (line[0] == "cgi_ext" && line.size() > 2)
			this->_cgi_ext.insert(std::make_pair(line[1], line[2]));
		else if (line[0] == "cgi_dir")
			this->_cgi_dir = line[1];
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
			if (verif.find("}") < 0)
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

std::vector<int> Socket::get_listen(void) const { return this->_listen; }
std::string Socket::get_ip(void) const { return this->_ip_address; }
std::vector<std::string> Socket::get_name(void) const { return this->_name; }
std::string Socket::get_root(void) const { return this->_root; }
std::map<int, std::string> Socket::get_error_pages(void) const { return this->_error_pages; }
std::string Socket::get_body_size(void) const { return this->_max_client_body_size;}
std::string Socket::get_cgi_dir(void) const { return this->_cgi_dir;}
std::map<std::string, std::string> Socket::get_cgi_ext(void) const { return this->_cgi_ext; }
bool Socket::get_autoindex(void) const { return this->_autoindex; }
std::vector<Location> Socket::get_location(void) const { return this->_location; }

std::ostream	&operator<<(std::ostream &o, Socket const &Socket) {
	o << BLUE << BOLD << " Socket :" << RESET << std::endl;
	if (Socket.get_listen().size())
	{
		o << "    listen = [";
		std::vector<int> listen = Socket.get_listen();
		for (size_t i = 0; i < listen.size(); i++)
			o << listen[i] << " | ";
		o << "]" << std::endl;
	}
	if (Socket.get_ip().size())
		o << "    ip = [" << Socket.get_ip() << "]" << std::endl;
	if (Socket.get_name().size())
	{
		o << "    Socket_name = [";
		std::vector<std::string> name = Socket.get_name();
		for (size_t i = 0; i < Socket.get_name().size(); i++)
			o << name[i] << " | ";
		o << "]" << std::endl;
	}
	if (Socket.get_root().size())
		o << "    root = [" << Socket.get_root() << "]" << std::endl;
	if (Socket.get_error_pages().size())
		o << "    error_page = [" << Socket.get_error_pages().size() << "]" << std::endl;
	if (Socket.get_body_size().size())
		o << "    body size = [" << Socket.get_body_size() << "]" << std::endl;
	if (Socket.get_cgi_dir().size() )
		o << "    cgi dir = [" << Socket.get_cgi_dir() << "]" << std::endl;
	if (Socket.get_cgi_ext().size())
		o << "    cgi ext = [" << Socket.get_cgi_ext().at(0) << "]" << std::endl;
	if (Socket.get_autoindex())
		o << "	autoindex" << Socket.get_autoindex() << std::endl;
	if (Socket.get_location().size())
	{
		std::vector<Location> loc = Socket.get_location();
		for (size_t i = 0; i < loc.size(); i++)
			o << loc[i];
	}
	return (o);
};
