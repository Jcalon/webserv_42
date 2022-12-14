/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcalon <jcalon@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/07 09:52:16 by mbascuna          #+#    #+#             */
/*   Updated: 2022/12/08 11:36:49 by jcalon           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/utils.hpp"

Response::Response(Request const &request, int error): _request(request)
{
	this->_http = "HTTP/1.1";
	this->_response = "\r\n";
	this->_content_length = 0;
	this->_content_location = "";
	this->_code_status = find_pair(error);
	this->_cgi = false;

	std::string 		page;
	std::string			line;

	page = "www/error_pages/" + ft_to_string(_code_status.first) + ".html";
	std::ifstream 		ifs(page.c_str());
	ifs.is_open();
	_response.clear();
	while (std::getline(ifs, line, char(ifs.eof())))
		this->_response.append(line);
	ifs.close();
	this->_content_length = _response.size();
	this->_content_type = "text/html"; // a modifier avec une fonction en fonction du ype
	this->_date = set_date();
	set_header();
}

Response::Response(Request const &request, Server const &server): _server(server), _request(request)
{
	this->_http = request.getRequest()._http;
	this->_response = "\r\n";
	this->_content_length = 0;
	this->_content_location = request.getRequest()._target;
	this->_path = get_index_path(request.getRequest()._target);
	this->_code_status = allow_method(request, server, request.getRequest()._target);
	this->_content_type = "";
	this->_header = "";
	this->_cgi = test_cgi(server, request.getRequest()._target);
	parse_body(request.getBody());
}

Response::~Response() {}

void Response::parse_body(std::string fields)
{
	if (this->_method == "PUT" || (this->_method == "POST" && this->_cgi == false))
	{
		this->_body.insert(make_pair("1", fields));
		return ;
	}
	if (fields != "" && _code_status.first == 200)
	{
		std::vector<std::string> tmp = ft_cpp_split(fields, "&");
		for (std::vector<std::string>::iterator it = tmp.begin(); it != tmp.end(); it++)
		{
			std::vector<std::string> map = ft_cpp_split(*it, "=");
			if (map.size() > 1)
				this->_body.insert(make_pair(map[0], map[1]));
			else
				this->_body.insert(make_pair("1", map[0]));
		}
	}
}

bool	Response::test_cgi(Server const &server, std::string loc_name)
{
	if ((this->_method == "POST" || this->_method == "GET") && this->_code_status.first == 200)
	{
		std::string ext = "";
		if (loc_name != "/")
		{
			std::vector<std::string> split = ft_cpp_split(loc_name, "/");
			loc_name = split.front();
			loc_name.insert(0, "/");
			loc_name += "/";
			ext = ft_cpp_split(split.back(), ".").back();
			ext.insert(0, ".");
		}
		if (is_cgi_in_location(server, loc_name, ext) || is_cgi_in_extension(server))
			return true;
	}
	return false;
}

bool	Response::is_cgi_in_extension(Server const &server)
{
	std::vector<Location> locations = server.get_location();
	std::string ext = ft_cpp_split(_content_location, ".").back();
	ext.insert(0, ".");
	for (std::vector<Location>::iterator it = locations.begin(); it != locations.end(); it++)
	{
		if (ext == it->get_name() && it->get_cgi_dir() != "")
		{
			this->_binary = it->get_cgi_dir();
			return true;
		}
	}
	return false;
}

bool	Response::is_cgi_in_location(Server const &server, std::string loc_name, std::string ext)
{
	std::vector<Location> locations = server.get_location();
	//si le path est une location
	for (std::vector<Location>::iterator it = locations.begin(); it != locations.end(); it++)
	{
		if (loc_name == it->get_name() && it->get_cgi_dir() != "")
		{
			if (ext != it->get_cgi_ext())
				return false;
			this->_binary = it->get_cgi_dir();
			return true;
		}
	}
	return false;
}

std::pair<int, std::string> Response::allow_method(Request const &request, Server const &server, std::string loc_name)
{
	if (loc_name != "/")
		loc_name = ft_cpp_split(loc_name, "/").front();
	loc_name.insert(0, "/");
	if (loc_name.find(".") == std::string::npos && *loc_name.end() != '/')
		loc_name.append("/");

	this->_method = request.getRequest()._method;
	if (!is_max_size_in_location(request, server, loc_name) || !is_max_size_in_extension(request, server))
		return find_pair(413);
	if (!is_allowed_in_location(server, loc_name) && !is_allowed_in_extension(server))
	{
		return find_pair(405);
	}
	if (this->_method == "PUT")
		return find_pair(201);
	return find_pair(200);
}

bool	Response::is_max_size_in_extension(Request const &request, Server const &server)
{
	std::vector<Location> locations = server.get_location();
	std::string max_size = server.get_body_size();
	std::string ext = ft_cpp_split(_content_location, ".").back();
	ext.insert(0, ".");
	for (std::vector<Location>::iterator it = locations.begin(); it != locations.end(); it++)
	{
		if (ext == it->get_name())
			max_size = it->get_body_size();
	}
	if ((max_size != "" && ft_str_to_int(max_size) >= request.getBody().length()) || max_size == "")
		return true;
	return false;
}

bool	Response::is_max_size_in_location(Request const &request, Server const &server, std::string loc_name)
{
	std::vector<Location> locations = server.get_location();
	std::string max_size = server.get_body_size();
	for (std::vector<Location>::iterator it = locations.begin(); it != locations.end(); it++)
	{
		if (loc_name == it->get_name())
			max_size = it->get_body_size();
	}
	if ((max_size != "" && ft_str_to_int(max_size) >= request.getBody().length()) || max_size == "")
		return true;
	return false;
}

bool	Response::is_allowed_in_extension(Server const &server)
{
	std::vector<Location> locations = server.get_location();
	std::vector<std::string> allow_method = server.get_allow_method();
	std::string ext = ft_cpp_split(_content_location, ".").back();
	int count = 0;
	if (_content_location.find(".") == std::string::npos)
		return false;
	ext.insert(0, ".");
	for (std::vector<Location>::iterator it = locations.begin(); it != locations.end(); it++)
	{
		if (ext == it->get_name())
		{
			allow_method = it->get_allow_method();
			count = 1;
		}
	}
	if (!count)
		return false;
	for (std::vector<std::string>::iterator it = allow_method.begin(); it != allow_method.end(); it++)
	{
		if (*it == this->_method)
			return true;
	}
	return false;
}

bool	Response::is_allowed_in_location(Server const &server, std::string loc_name)
{
	std::vector<Location> locations = server.get_location();
	std::vector<std::string> allow_method = server.get_allow_method();
	//si le path est une location
	for (std::vector<Location>::iterator it = locations.begin(); it != locations.end(); it++)
	{
		if (loc_name == it->get_name())
			allow_method = it->get_allow_method();
	}
	for (std::vector<std::string>::iterator it = allow_method.begin(); it != allow_method.end(); it++)
	{
		if (*it == this->_method)
			return true;
	}
	return false;
}

void Response::call_method()
{
	if (_code_status.first != 200 && _code_status.first != 201)
	{
		load_error_pages();
		this->_content_length = _response.size();
		this->_content_type = init_mime_types(); // a modifier avec une fonction en fonction du ype
		this->_date = set_date();
		set_header();
	}
	else if (this->_method == "GET")
		run_get_method();
	else if (this->_method == "POST")
		run_post_method();
	else if (this->_method == "HEAD")
		run_head_method();
	else if (this->_method == "PUT")
		run_put_method();
	else if (this->_method == "DELETE")
		run_delete_method();
}

void Response::load_error_pages()
{
	std::string			line;
	std::string 		page;
	std::map<int, std::string> error_pages = _server.get_error_pages();
	if (error_pages.size() > 0)
	{
		page = error_pages.find(_code_status.first)->second;
		std::string tmp = ft_cpp_split(page, "/").front();
		if (tmp != _server.get_root().substr(0, _server.get_root().length() - 1))
			page.insert(0, _server.get_root() + "/");
	}
	else
		page = "www/error_pages/" + ft_to_string(_code_status.first) + ".html";

	std::ifstream 		ifs(page.c_str());

	ifs.is_open();
	_response.clear();
	while (std::getline(ifs, line, char(ifs.eof())))
		this->_response.append(line);
	ifs.close();
}

void Response::run_get_method(void)
{
	std::string			line;
	bool 				autoindex = _server.get_autoindex();
	std::vector<Location> locations = _server.get_location();
	int index = 0;

	if (_cgi == true)
		run_cgi_method();
	else
	{
		std::string parse_loc = _content_location;
		if (parse_loc[parse_loc.size() - 1] != '/')
			parse_loc.append("/");
		for (std::vector<Location>::iterator it = locations.begin(); it != locations.end(); it++)
		{
			if (parse_loc == it->get_name())
			{
				autoindex = it->get_autoindex();
				if (it->get_index() != "")
					index = 1;
			}
		}
		std::string ext = ft_cpp_split(_request.getRequest()._target, ".").back();
		if (autoindex && !index && ext != "ico")
		{
			Autoindex autoindex(_server, _path);
			this->_response = autoindex.get_html();
		}
		else
		{
			std::ifstream		ifs(_path.c_str());
			DIR				*dir;
			dir = opendir(_path.c_str());
			if ((!ifs.is_open() || dir != NULL))
				this->_code_status = find_pair(404);
			if (access(_path.c_str(), F_OK) == 0 && access(_path.c_str(), R_OK) < 0)
				this->_code_status = find_pair(403);
			while (std::getline(ifs, line, char(ifs.eof())))
				this->_response.append(line);
			ifs.close();
		}
		if (_code_status.first != 200)
			load_error_pages();
		this->_content_length = _response.size();
		this->_content_type = init_mime_types(); // a modifier avec une fonction en fonction du ype
		this->_date = set_date();

		set_header();
	}
}

void Response::run_head_method(void)
{
	run_get_method();
}

void Response::run_delete_method(void)
{
	if (remove(_path.c_str()) == 0)
		this->_code_status = find_pair(204);
	else
		this->_code_status = find_pair(404);
	if (_code_status.first != 204)
		load_error_pages();
	this->_content_length = _response.size();
	this->_content_type = init_mime_types(); // a modifier avec une fonction en fonction du ype
	this->_date = set_date();
	set_header();
}

void Response::run_post_method(void)
{
	if (_code_status.first == 200 && _cgi == true)
		run_cgi_method();
	else if (_request.getFilename() != "")
	{
		_code_status = find_pair(201);
		_path += "/" + _request.getFilename();
		if (is_readable(_path.c_str()))
			_code_status = find_pair(204);

		std::ofstream		ofs(_path.c_str());
		std::string	line;

		if (!ofs.is_open())
			_code_status = find_pair(403);
		ofs << this->_body["1"];
		ofs.close();

		this->_response = "";
		if (_code_status.first != 200)
			load_error_pages();
		this->_content_length = _response.size();
		this->_content_type = "text/html"; // a modifier avec une fonction en fonction du ype
		this->_date = set_date();
		set_header();
	}
	else
	{
		this->_response = "";
		this->_content_length = _response.size();
		this->_date = set_date();
		set_header();
	}
}

void Response::run_cgi_method(void)
{
	std::string 	output;

	CGI cgi(this->_request, this->_server, this->_binary, this->_path);
	output = cgi.interpreter();

	if (output != "500")
	{
		std::istringstream			stream(output);
		std::string					line;
		bool						header = true;
		std::string					headerfromcgi = "";
		this->_response = "";

		while (std::getline(stream, line))
		{
			if (line == "\r")
				header = false;
			else
			{
				if (header && ft_cpp_split(line, ":").size() == 2)
					headerfromcgi += line;
				else if (!header)
					this->_response += line;
			}
		}
		this->_content_length = _response.size();
		this->_date = set_date();
		set_header();
		this->_header += headerfromcgi;
	}
	else
	{
		this->_code_status = find_pair(200);
		load_error_pages();
		this->_content_length = _response.size();
		this->_content_type = init_mime_types(); // a modifier avec une fonction en fonction du ype
		this->_date = set_date();
		set_header();
	}
}


void Response::run_put_method(void)
{
	DIR				*dir;
	dir = opendir(_path.c_str());
	if (dir != NULL)
		_code_status = find_pair(409);
	if (_code_status.first == 201)
	{
		std::ofstream		ofs(_path.c_str());
		std::string	line;

		if (!ofs.is_open())
			_code_status = find_pair(403);
		ofs << this->_body["1"];
		ofs.close();
	}
	this->_response = "";
	this->_content_length = _response.size();
	this->_content_type = "text/html"; // a modifier avec une fonction en fonction du ype
	this->_date = set_date();
	set_header();
}

void	Response::set_header(void)
{
	this->_header = this->_http + " " + this->_code_status.second;
	this->_header += "\r\nContent-Length: " + ft_to_string(this->_content_length);
	if (this->_content_location != "")
		this->_header += "\r\nContent-Location: " + this->_content_location;
	if (this->_cgi == false)
		this->_header += "\r\nContent-Type: " + this->_content_type;
	this->_header += "\r\nDate: " + this->_date;
	this->_header += "\r\nServer: webserv";
}

std::string	Response::set_date(void)
{
	char			buffer[100];
	struct timeval	tv;
	struct tm		*tm;

	gettimeofday(&tv, NULL);
	tm = gmtime(&tv.tv_sec);
	strftime(buffer, 100, "%a, %d %b %Y %H:%M:%S GMT", tm);
	return buffer;
}

std::string Response::get_index_path(std::string location) const
{
	std::vector<Location> 		locations = _server.get_location();
	std::vector<std::string> 	split_path = ft_cpp_split(location, "/");
	std::string 				path = _server.get_root();

	if (path.find("./") == std::string::npos)
		path.insert(0, "./");
	if (split_path.size() < 1)
	{
		for (std::vector<Location>::iterator it = locations.begin(); it != locations.end(); it++)
		{
			if (it->get_name() == location)
			{
				if (it->get_root() != "")
				{
					path.clear();
					if (it->get_root().find("./") == std::string::npos)
						path = "./" + it->get_root();
					else
						path = it->get_root();
				}
				if (it->get_index() != "")
					path += "/" + it->get_index();
				if (path != "")
					return path;
			}
		}
		if (_server.get_root().rfind("/") != _server.get_root().length() - 1)
			return _server.get_root() + "/" + _server.get_index();
		return _server.get_root() + _server.get_index();
	}
	for (std::vector<std::string>::iterator it = split_path.begin(); it != split_path.end(); it++)
		it->insert(0, "/");
	if (split_path[0].find(".") == std::string::npos && split_path[0].rfind("/") != split_path[0].length() - 1)
		split_path[0] += "/";
	for (std::vector<Location>::iterator it = locations.begin(); it != locations.end(); it++)
	{
		if (it->get_name() == split_path[0])
		{
			if (it->get_root() != "")
			{
				path = it->get_root();
			}
			std::string loc;

			if (path.rfind("/") == path.length() - 1)
			{
				loc = path + it->get_name().substr(1);
			}
			else
			{
				loc = path + it->get_name();
			}
			DIR				*dir;
			dir = opendir(loc.c_str());
			if (it->get_index() != "" && dir != NULL && split_path.size() < 2)
			{
				if (path.rfind("/") == path.length() - 1)
					path += it->get_name().substr(1) + it->get_index();
				else
					path += it->get_name() + it->get_index();
			}
			else if (dir != NULL)
			{
				if (path.rfind("/") == path.length() - 1)
					path += it->get_name().substr(1);
				else
					path += it->get_name();
			}
			if (split_path.size() >= 2)
			{
				int i = 1;
				for (std::vector<std::string>::iterator itsplit = split_path.begin() + i; itsplit != split_path.end(); itsplit++)
				{
					if (itsplit->rfind("/") != itsplit->length() - 1 && itsplit != --split_path.end() && itsplit->find(".") == std::string::npos)
					{
						path += *itsplit + "/";
					}
					else
					{
						if (path.rfind("/") == path.length() - 1)
							path += itsplit->substr(1);
						else
							path += *itsplit;
					}
				}
				DIR				*tst;
				tst = opendir(path.c_str());
				if (tst != NULL && it->get_index() != "")
				{
					if (path.rfind("/") != path.length() - 1)
						path += "/";
					path += it->get_index();
				}
			}
			else if (dir == NULL && it->get_index() != "")
			{
				if (path.rfind("/") != path.length() - 1)
				{
					path += "/";
				}
				path += it->get_index();
			}
			return path;
		}
	}
	return path + split_path[0];
}

const char *Response::FileNotOpen::what() const throw()
{
	return ("Error opening html file");
}

std::pair<int, std::string> Response::find_pair(int code)
{
	std::map<int, std::string> map_error;

	map_error.insert(std::make_pair(200, "200 OK"));
	map_error.insert(std::make_pair(201, "201 Created"));
	map_error.insert(std::make_pair(204, "204 No Content"));
	map_error.insert(std::make_pair(300, "300 Multiple Choices"));
	map_error.insert(std::make_pair(301, "301 Moved Permanently"));
	map_error.insert(std::make_pair(302, "302 Found"));
	map_error.insert(std::make_pair(303, "303 See Other"));
	map_error.insert(std::make_pair(307, "307 Temporary Redirect"));
	map_error.insert(std::make_pair(400, "400 Bad Request"));
	map_error.insert(std::make_pair(403, "403 Forbidden"));
	map_error.insert(std::make_pair(404, "404 Not Found"));
	map_error.insert(std::make_pair(405, "405 Method Not Allowed"));
	map_error.insert(std::make_pair(408, "408 Request Timeout"));
	map_error.insert(std::make_pair(409, "409 Conflict"));
	map_error.insert(std::make_pair(411, "411 Length Required"));
	map_error.insert(std::make_pair(413, "413 Request Entity Too Large"));
	map_error.insert(std::make_pair(414, "414 Request-URI Too Long"));
	map_error.insert(std::make_pair(500, "500 Internal Server Error"));
	map_error.insert(std::make_pair(502, "502 Bad Gateway"));
	map_error.insert(std::make_pair(505, "505 HTTP Version Not Supported"));

	std::map<int, std::string>::iterator it = map_error.find(code);
	return (std::make_pair(it->first, it->second));
}

std::string		Response::init_mime_types()
{
    std::map<std::string, std::string> mime_types;

	mime_types[".aac"]      = "audio/aac";
	mime_types[".abw"]      = "application/x-abiword";
	mime_types[".arc"]      = "application/octet-stream";
	mime_types[".avi"]      = "video/x-msvideo";
	mime_types[".azw"]      = "application/vnd.amazon.ebook";
	mime_types[".bin"]      = "application/octet-stream";
	mime_types[".bz"]       = "application/x-bzip";
	mime_types[".bz2"]      = "application/x-bzip2";
	mime_types[".csh"]      = "application/x-csh";
	mime_types[".css"]      = "text/css";
	mime_types[".csv"]      = "text/csv";
	mime_types[".doc"]      = "application/msword";
	mime_types[".epub"]     = "application/epub+zip";
	mime_types[".gif"]      = "image/gif";
	mime_types[".htm"]      = "text/html";
	mime_types[".html"]     = "text/html";
	mime_types[".ico"]      = "image/x-icon";
	mime_types[".ics"]      = "text/calendar";
	mime_types[".jar"]      = "Temporary Redirect";
	mime_types[".jpeg"]     = "image/jpeg";
	mime_types[".jpg"]      = "image/jpeg";
	mime_types[".js"]       = "application/js";
	mime_types[".json"]     = "application/json";
	mime_types[".mid"]      = "audio/midi";
	mime_types[".midi"]     = "audio/midi";
	mime_types[".mpeg"]     = "video/mpeg";
	mime_types[".mpkg"]     = "application/vnd.apple.installer+xml";
	mime_types[".odp"]      = "application/vnd.oasis.opendocument.presentation";
	mime_types[".ods"]      = "application/vnd.oasis.opendocument.spreadsheet";
	mime_types[".odt"]      = "application/vnd.oasis.opendocument.text";
	mime_types[".oga"]      = "audio/ogg";
	mime_types[".ogv"]      = "video/ogg";
	mime_types[".ogx"]      = "application/ogg";
	mime_types[".png"]      = "image/png";
	mime_types[".pdf"]      = "application/pdf";
	mime_types[".ppt"]      = "application/vnd.ms-powerpoint";
	mime_types[".rar"]      = "application/x-rar-compressed";
	mime_types[".rtf"]      = "application/rtf";
	mime_types[".sh"]       = "application/x-sh";
	mime_types[".svg"]      = "image/svg+xml";
	mime_types[".swf"]      = "application/x-shockwave-flash";
	mime_types[".tar"]      = "application/x-tar";
	mime_types[".tif"]      = "image/tiff";
	mime_types[".tiff"]     = "image/tiff";
	mime_types[".ttf"]      = "application/x-font-ttf";
	mime_types[".txt"]      = "text/plain";
	mime_types[".vsd"]      = "application/vnd.visio";
	mime_types[".wav"]      = "audio/x-wav";
	mime_types[".weba"]     = "audio/webm";
	mime_types[".webm"]     = "video/webm";
	mime_types[".webp"]     = "image/webp";
	mime_types[".woff"]     = "application/x-font-woff";
	mime_types[".xhtml"]    = "application/xhtml+xml";
	mime_types[".xls"]      = "application/vnd.ms-excel";
	mime_types[".xml"]      = "application/xml";
	mime_types[".xul"]      = "application/vnd.mozilla.xul+xml";
	mime_types[".zip"]      = "application/zip";
	mime_types[".3gp"]      = "video/3gpp audio/3gpp";
	mime_types[".3g2"]      = "video/3gpp2 audio/3gpp2";
	mime_types[".7z"]       = "application/x-7z-compressed";

	std::string ext = ft_cpp_split(_content_location, ".").back();
	ext.insert(0, ".");
	std::map<std::string, std::string>::iterator it = mime_types.find(ext);
	if (it != mime_types.end())
		return it->second;
	else
		return "text/html";
}

std::string Response::get_header(void) const { return this->_header; }
std::string Response::get_response(void) const { return this->_response; }

