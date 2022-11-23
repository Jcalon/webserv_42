/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Location.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbascuna <mbascuna@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/21 12:22:21 by mbascuna          #+#    #+#             */
/*   Updated: 2022/11/22 15:55:54 by mbascuna         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/utils.hpp"

Location::Location(void) {}

Location::~Location(void) {}

std::vector<std::string>::iterator Location::parse_location(std::vector<std::string>::iterator start, std::vector<std::string> file)
{
	std::vector<std::string> line;
	line = ft_cpp_split(*start, WHITESPACES);
	this->_name = line[1];
	start++;
	for (;start != file.end(); ++start)
	{
		line = ft_cpp_split(*start, WHITESPACES);
		if (line[0] == "root")
			this->_root = line[1];
		else if (line[0] == "upload")
			this->_upload = line[1];
		else if (line[0] == "allow_method")
		{
			for (size_t i = 0; i < line.size(); i++)
				this->_allow_method.push_back(line[i]);
		}
		else if (line[0] == "client_max_body_size")
			this->_max_client_body_size = line[1];
		else if (line[0] == "index")
			this->_index = line[1];
		else if (line[0] == "cgi_ext")
			this->_cgi_ext.insert(std::make_pair(line[1], line[2]));
		else if (line[0] == "cgi_dir")
			this->_cgi_dir = line[1];
		else if (line[0] == "autoindex")
			(line[1] == "on" )? this->_autoindex = true : this->_autoindex = false;
		else
			break ;
	}
	return start;

}

std::string Location::get_name(void) const { return this->_name; }
std::string Location::get_root(void) const { return this->_root; }
std::string Location::get_upload(void) const { return this->_upload; }
std::vector<std::string> Location::get_allow_method(void) const { return this->_allow_method; }
std::string Location::get_body_size(void) const { return this->_max_client_body_size;}
std::string Location::get_index(void) const { return this->_index;}
std::map<std::string, std::string> Location::get_cgi_ext(void) const { return this->_cgi_ext; }
bool Location::get_autoindex(void) const { return this->_autoindex; }

std::ostream	&operator<<(std::ostream &o, Location const &location) {
	o << YELLOW << BOLD << "   Location :  " << RESET << location.get_name() << std::endl;
	if (location.get_root().size())
		o << "     root = [" << location.get_root() << "]" << std::endl;
	if (location.get_upload().size())
		o << "     upload = [" << location.get_upload() << "]" << std::endl;
	if (location.get_allow_method().size() > 1)
	{
		o << "     allow method = [";
		std::vector<std::string> method = location.get_allow_method();
		for (size_t i = 1; i < method.size(); i++)
			o << method[i] << " | ";
		o << "]" << std::endl;
	}
	if (location.get_body_size().size() )
		o << "     body size = [" << location.get_body_size() << "]" << std::endl;
	if (location.get_index().size() )
		o << "     index = [" << location.get_index() << "]" << std::endl;
	if (location.get_cgi_ext().size())
		o << "     cgi ext = [" << location.get_cgi_ext().size() << "]" << std::endl;
	// if (location.get_autoindex())
	o << "     autoindex = [" << location.get_autoindex() << "]" << std::endl;
	return (o);
};

