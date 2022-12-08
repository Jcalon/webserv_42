

#include "../includes/utils.hpp"

Server::Server(void) {
	this->_ip_address = "0.0.0.0";
	this->_index = "";
	this->_listen.push_back("80");
	this->_autoindex = false;
	this->_cgi_dir = "";
	this->_max_client_body_size = "";
	this->_root = "./";
	this->_is_error = false;
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
	this->_error_pages.insert(std::make_pair(201, "www/error_pages/201.html"));
	this->_error_pages.insert(std::make_pair(204, "www/error_pages/204.html"));
}

void Server::init_allow_methods(void)
{
	this->_allow_method.push_back("GET");
	this->_allow_method.push_back("POST");
	this->_allow_method.push_back("DELETE");
	this->_allow_method.push_back("HEAD");
	this->_allow_method.push_back("PUT");
}

bool Server::check_listen(std::vector<std::string> line)
{
	this->_listen.pop_back();
	if (line.size() > 3)
		return true;
	for (size_t i = 1; i < line.size(); i++)
	{
		if (line[i].find(":") != std::string::npos)
		{
			std::vector<std::string> tmp = ft_cpp_split(line[i], ":");
			if (!ft_is_alnum(tmp[0]) || (!ft_is_alnum(tmp[1]) && tmp[1] != "default"))
				return true;
			this->_ip_address = tmp[0];
			this->_listen.push_back(tmp[1]);
		}
		else
		{
			if (!ft_is_alnum(line[i]) && line[i] != "default")
				return true;
			this->_listen.push_back(line[i]);
		}
	}
	return false;
}

bool Server::check_method(std::vector<std::string> line)
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

bool Server::check_error(std::vector<std::string> line)
{
	if (line.size() > 3)
		return true;
	if (!ft_is_num(line[1]))
		return true;
	std::map<int, std::string>::iterator it = this->_error_pages.find(std::atoi(line[1].c_str()));
	if (it != _error_pages.end())
		it->second = line[2];
	else
		this->_error_pages.insert(std::make_pair(std::atoi(line[1].c_str()), line[2]));
	return false;
}

bool Server::check_auto(std::vector<std::string> line)
{
	if (line[1] != "on" && line[1] != "off")
		return true;
	if (line.size() > 2)
		return true;
	(line[1] == "on") ? this->_autoindex = true : this->_autoindex = false;
	return false;
}

//surement appeler des fonctions pour chaque item . fonctions qui checkeront les cas d'erreur et syntax pour cahcun
std::vector<std::string>::iterator Server::parse_server(std::vector<std::string>::iterator start, std::vector<std::string> file)
{
	int nb_line = 0;
	start++;
	for (;start != file.end(); start++)
	{
		std::vector<std::string> line;
		line = ft_cpp_split(*start, WHITESPACES);
		nb_line++;
		if (line[0] == "listen")
		{
			this->_is_error = check_listen(line);
			if (this->_is_error == true)
				break;
		}
		else if (line[0] == "server_name")
			this->_name.push_back(line[1]);
		else if (line[0] == "allow_method")
		{
			this->_is_error = check_method(line);
			if (this->_is_error == true)
				break;
		}
		else if (line[0] == "max_client_body_size")
			this->_max_client_body_size = line[1];
		else if (line[0] == "root")
		{
			if (line.size() > 2)
			{
				this->_is_error = true;
				break;
			}
			this->_root = line[1];
			if (this->_root.find(".") == std::string::npos && *this->_root.end() != '/' && this->_root.size() > 1)
				this->_root.append("/");
		}
		else if (line[0] == "error_page" && line.size() > 2)
		{
			this->_is_error = check_error(line);
			if (this->_is_error == true)
				break;
		}
		else if (line[0] == "cgi_ext")
		{
			if (line.size() > 2)
			{
				this->_is_error = true;
				break;
			}
			this->_cgi_ext = line[1];
		}
		else if (line[0] == "cgi_dir")
		{
			if (line.size() > 2)
			{
				this->_is_error = true;
				break;
			}
			this->_cgi_dir = line[1];
		}
		else if (line[0] == "index")
		{
			if (line.size() > 2)
			{
				this->_is_error = true;
				break;
			}
			this->_index = line[1];
		}
		else if (line[0] == "location")
		{
			Location location(*this);
			if (line[line.size() - 1] != "{")
			{
				std::cout << RED << "ERROR: " << RESET << "Location should open with {..." << std::endl;
				this->_is_error = true;
				break;
			}
			start = location.parse_location(start, file);
			this->_is_error = location.get_error();
			if (this->_is_error == true)
				break;
			this->_location.push_back(location);
			std::string verif = *start;
			if (*start == file[file.size() - 1])
			{
				std::cout << RED << "ERROR: " << RESET << "Server should close with }..." << std::endl;
				this->_is_error = true;
				break ;
			}
			if (verif.find("}") == std::string::npos )
			{
				this->_is_error = true;
				break ;
			}
		}
		else if (line[0] == "autoindex")
		{
			this->_is_error = check_auto(line);
			if (this->_is_error == true)
				break;
		}
		else
			break ;

	}
	if (nb_line <= 1)
		this->_is_error = true;
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
std::string						 	Server::get_cgi_ext(void) const { return this->_cgi_ext; }
bool 								Server::get_autoindex(void) const { return this->_autoindex; }
std::vector<Location> 				Server::get_location(void) const { return this->_location; }
std::vector<std::string> 			Server::get_allow_method(void) const { return this->_allow_method; }
bool								Server::get_error(void) const { return this->_is_error; }

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
		for (size_t i = 0; i < method.size(); i++)
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
		o << "    cgi ext = [" << Server.get_cgi_ext() << "]" << std::endl;
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
