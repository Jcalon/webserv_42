/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Location.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbascuna <mbascuna@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/01 16:05:58 by mbascuna          #+#    #+#             */
/*   Updated: 2022/12/08 11:14:27 by mbascuna         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/utils.hpp"

Location::Location(Server &serv): _root(""), _index(""), _max_client_body_size(""), _cgi_dir(serv.get_cgi_dir())
{
	this->_autoindex = false;
	this->_allow_method = serv.get_allow_method();
	this->_is_error = false;

}

Location::~Location(void) {}

bool Location::check_method(std::vector<std::string> line)
{
	std::vector<std::string> check;
	check.push_back("GET");
	check.push_back("POST");
	check.push_back("PUT");
	check.push_back("HEAD");
	check.push_back("DELETE");
	this->_allow_method.clear();
	for (size_t i = 1; i < line.size(); i++)
	{
		for (std::vector<std::string>::iterator it = check.begin(); it != check.end(); it++)
		{
			if (*it == line[i])
				this->_allow_method.push_back(line[i]);
		}
	}
	if (this->_allow_method.size() != line.size() - 1)
		return true;
	return false;

}

bool Location::check_size(std::vector<std::string> line)
{
	if (line.size() > 2)
		return true;
	if (!ft_is_num(line[1]))
		return true;
	this->_max_client_body_size = line[1];
	return false;
}

bool Location::check_auto(std::vector<std::string> line)
{
	if (line[1] != "on" && line[1] != "off")
		return true;
	if (line.size() > 2)
		return true;
	(line[1] == "on") ? this->_autoindex = true : this->_autoindex = false;
	return false;
}

std::vector<std::string>::iterator Location::parse_location(std::vector<std::string>::iterator start, std::vector<std::string> file)
{
	std::vector<std::string> line;
	line = ft_cpp_split(*start, WHITESPACES);
	this->_name = line[1];
	if (this->_name.find(".") == std::string::npos && *this->_name.end() != '/' && this->_name.size() > 1)
		this->_name.append("/");
	start++;
	for (;start != file.end() - 1; ++start)
	{
		line = ft_cpp_split(*start, WHITESPACES);
		if (line[0] == "root")
			this->_root = line[1];
		else if (line[0] == "upload")
			this->_upload = line[1];
		else if (line[0] == "allow_method")
		{
			this->_is_error = check_method(line);
			if (this->_is_error == true)
				break;
		}
		else if (line[0] == "client_max_body_size")
		{
			this->_is_error = check_size(line);
			if (this->_is_error == true)
				break;
		}
		else if (line[0] == "index")
			this->_index = line[1];
		else if (line[0] == "cgi_ext")
			this->_cgi_ext = line[1];
		else if (line[0] == "cgi_dir")
			this->_cgi_dir = line[1];
		else if (line[0] == "autoindex")
		{
			this->_is_error = check_auto(line);
			if (this->_is_error == true)
				break;
		}
		else
			break ;
	}
	return start;
}

std::string 				Location::get_name(void) const { return this->_name; }
std::string 				Location::get_root(void) const { return this->_root; }
std::string 				Location::get_upload(void) const { return this->_upload; }
std::vector<std::string> 	Location::get_allow_method(void) const { return this->_allow_method; }
std::string 				Location::get_body_size(void) const { return this->_max_client_body_size;}
std::string 				Location::get_index(void) const { return this->_index;}
std::string 				Location::get_cgi_dir(void) const { return this->_cgi_dir;}
std::string					Location::get_cgi_ext(void) const { return this->_cgi_ext; }
bool						Location::get_autoindex(void) const { return this->_autoindex; }
bool						Location::get_error(void) const { return this->_is_error; }


std::ostream	&operator<<(std::ostream &o, Location const &location) {
	o << YELLOW << BOLD << "   Location :  " << RESET << location.get_name() << std::endl;
	if (location.get_root().size())
		o << "     root = [" << location.get_root() << "]" << std::endl;
	if (location.get_upload().size())
		o << "     upload = [" << location.get_upload() << "]" << std::endl;
	if (location.get_allow_method().size() >= 1)
	{
		o << "     allow method = [";
		std::vector<std::string> method = location.get_allow_method();
		for (size_t i = 0; i < method.size(); i++)
			o << method[i] << " | ";
		o << "]" << std::endl;
	}
	if (location.get_body_size().size() )
		o << "     body size = [" << location.get_body_size() << "]" << std::endl;
	if (location.get_index().size() )
		o << "     index = [" << location.get_index() << "]" << std::endl;
	if (location.get_cgi_ext().size())
		o << "     cgi ext = [" << location.get_cgi_ext().size() << "]" << std::endl;
	o << "     autoindex = [" << location.get_autoindex() << "]" << std::endl;
	return (o);
};

