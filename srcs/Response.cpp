/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcalon <jcalon@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/22 17:48:13 by mbascuna          #+#    #+#             */
/*   Updated: 2022/11/30 14:51:06 by jcalon           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/utils.hpp"

Response::Response(Request const &request, Server const &server)
{
	this->_http = request.getRequest()._http;
	this->_response = "\r\n";
	this->_content_length = 0;
	if (request.getRequest()._target == "/")
		this->_content_location = server.get_index();
	else
		this->_content_location = server.get_index_path(request.getRequest()._target);
	this->_code_status = allow_method(request, server, request.getRequest()._target);
	this->_content_type = "";
	this->_header = "";
	parse_body(request.getBody());
}

Response::~Response() {}

void Response::parse_body(std::string fields)
{
	if (this->_method == "PUT")
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
			this->_body.insert(make_pair(map[0], map[1]));
		}
		// for (std::map<std::string, std::string>::iterator it = _body.begin(); it != _body.end(); it++)
		// 	std::cout << YELLOW << "map[" << it->first << "] = " << it->second << std::endl;
	}
}

std::pair<int, std::string> Response::allow_method(Request const &request, Server const &server, std::string loc_name)
{
	if (loc_name != "/")
		loc_name = ft_cpp_split(loc_name, "/").front();
	loc_name.insert(0, "/");

	this->_method = request.getRequest()._method;
	if (!is_allowed_in_location(server, loc_name) && !is_allowed_in_extension(server))
		return find_pair(405);
	if (this->_method == "PUT")
		return find_pair(201);
	return find_pair(200);
}

bool	Response::is_allowed_in_extension(Server const &server)
{
	std::vector<Location> locations = server.get_location();
	std::vector<std::string> allow_method = server.get_allow_method();
	std::string ext = ft_cpp_split(_content_location, ".").back();
	ext.insert(0, ".");
	for (std::vector<Location>::iterator it = locations.begin(); it != locations.end(); it++)
	{
		if (ext == it->get_name())
			allow_method = it->get_allow_method();
	}
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
	if (this->_method == "GET")
		run_get_method();
	else if (this->_method == "POST")
		run_post_method();
	else if (this->_method == "HEAD")
		run_head_method();
	else if (this->_method == "PUT")
		run_put_method();
	// else if (method == "DELETE")
	// 	run_delete_method();
}

void Response::load_error_pages()
{
	std::string			line;
	std::string 		page = "www/error_pages/" + ft_to_string(_code_status.first) + ".html";
	std::ifstream 		ifs(page.c_str());

	ifs.is_open();
	_response.clear();
	while (std::getline(ifs, line, char(ifs.eof())))
		this->_response.append(line);
	ifs.close();
}

void Response::run_get_method(void)
{
	std::ifstream		ifs(_content_location.c_str());
	std::string			line;

	if (!ifs.is_open())
		this->_code_status = find_pair(404);
	while (std::getline(ifs, line, char(ifs.eof())))
		this->_response.append(line);
	ifs.close();
	if (_code_status.first != 200)
		load_error_pages();
	this->_content_length = _response.size();
	this->_content_type = init_mime_types(); // a modifier avec une fonction en fonction du ype
	this->_date = set_date();

	set_header();
	//gerer le autoindex ?
}

void Response::run_head_method(void)
{
	run_get_method();
}

void Response::run_post_method(void)
{
	if (_code_status.first == 200)
	{
		std::ifstream		ifs(_content_location.c_str());
		std::string	line;

		if (!ifs.is_open())
			throw Config::FileNotOpen();
		while (std::getline(ifs, line, char(ifs.eof())))
			this->_response = line;

		for (std::map<std::string, std::string>::iterator it = _body.begin(); it != _body.end(); it++)
		{
			std::string balise = "<div id=\"com\">";
			size_t pos = _response.find(balise);
			std::string str = "<h4>" + it->first + "</h4><p>: " + it->second + "</p>\n";
			_response.insert(pos+balise.size(), str);
		}
		ifs.close();

		std::ofstream		ofs(_content_location.c_str());
		ofs << _response;
		ofs.close();
	}
	else
		this->_response = "";

	this->_content_length = _response.size();
	this->_content_type = "text/html"; // a modifier avec une fonction en fonction du ype
	this->_date = set_date();
	set_header();
}

void Response::run_put_method(void)
{
	if (_code_status.first == 201)
	{
		std::ofstream		ofs(_content_location.c_str());
		std::string	line;

		if (!ofs.is_open())
			throw Config::FileNotOpen();
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
	this->_header += "\r\nContent-Location: " + this->_content_location;
	this->_header += "\r\nContent-Type: " + this->_content_type;
	this->_header += "\r\nDate: " + this->_date;
	this->_header += "\nServer: webserv\r\n";
	// this->_header += RED "\nTransfer-Encoding: ??????\n\r" RESET;
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
	map_error.insert(std::make_pair(404, "404 Not Found"));
	map_error.insert(std::make_pair(405, "405 Method Not Allowed"));
	map_error.insert(std::make_pair(408, "408 Request Timeout"));
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
