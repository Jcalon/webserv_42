/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcalon <jcalon@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/22 17:48:13 by mbascuna          #+#    #+#             */
/*   Updated: 2022/11/28 16:36:50 by jcalon           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/utils.hpp"

Response::Response(Request const &request, Server const &server)
{
	//mettre dans map quand on aura fait la map des codes erreurs
	// this->_code = allow_method(request);
	this->_code_status = allow_method(request);
	this->_http = request.getRequest()._http;
	this->_response = "";
	this->_content_length = 0;
	if (request.getRequest()._target == "/" || request.getRequest()._target == "/favicon.ico")
		this->_content_location = server.get_index();
	else
		this->_content_location = request.getRequest()._target;
	this->_content_type = "";
	this->_header = "";
	// this->_body = parse_body(request.getFields());
	parse_body(request.getFields());
}


	// this->_error_pages.insert(std::make_pair(200, "200 OK"));
	// this->_error_pages.insert(std::make_pair(201, "201 Created"));
	// this->_error_pages.insert(std::make_pair(204, "204 No Content"));
	// this->_error_pages.insert(std::make_pair(300, "300 Multiple Choices"));
	// this->_error_pages.insert(std::make_pair(301, "301 Moved Permanently"));
	// this->_error_pages.insert(std::make_pair(302, "302 Found"));
	// this->_error_pages.insert(std::make_pair(303, "303 See Other"));
	// this->_error_pages.insert(std::make_pair(307, "307 Temporary Redirect"));
	// this->_error_pages.insert(std::make_pair(400, "400 Bad Request"));
	// this->_error_pages.insert(std::make_pair(404, "404 Not Found"));
	// this->_error_pages.insert(std::make_pair(405, "405 Method Not Allowed"));
	// this->_error_pages.insert(std::make_pair(408, "408 Request Timeout"));
	// this->_error_pages.insert(std::make_pair(411, "411 Length Required"));
	// this->_error_pages.insert(std::make_pair(413, "413 Request Entity Too Large"));
	// this->_error_pages.insert(std::make_pair(414, "414 Request-URI Too Long"));
	// this->_error_pages.insert(std::make_pair(500, "500 Internal Server Error"));
	// this->_error_pages.insert(std::make_pair(502, "502 Bad Gateway"));
	// this->_error_pages.insert(std::make_pair(505, "505 HTTP Version Not Supported"));

Response::~Response() {
}

void Response::parse_body(std::vector<std::string> fields)
{
	if (!fields.back().empty())
	{
		std::vector<std::string> tmp = ft_cpp_split(fields.back(), "&");
		for (std::vector<std::string>::iterator it = tmp.begin(); it != tmp.end(); it++)
		{
			std::vector<std::string> map = ft_cpp_split(*it, "=");
			this->_body.insert(make_pair(map[0], map[1]));
		}
		// for (std::map<std::string, std::string>::iterator it = _body.begin(); it != _body.end(); it++)
		// 	std::cout << YELLOW << "map[" << it->first << "] = " << it->second << std::endl;
	}

}


std::pair<int, std::string> Response::allow_method(Request const &request)
{
	this->_method = request.getRequest()._method;
	// if (this->allow_method.at(_method))
	// 	std::cout << RED << " COUCOU \n";
	//if _method n'est dans les method allow
		//envoyer code erreur correspondant
	return find_pair(200);
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

std::string Response::get_header(void) const { return this->_header; }
std::string Response::get_response(void) const { return this->_response; }


void Response::call_method()
{
	if (this->_method == "GET")
		run_get_method();
	else if (this->_method == "POST")
		run_post_method();
	// else if (method == "DELETE")
	// 	run_delete_method();
}

void Response::run_get_method(void)
{
	std::ifstream		ifs(_content_location.c_str());
	std::string	line;

	if (!ifs.is_open())
		throw Config::FileNotOpen();
	while (std::getline(ifs, line, char(ifs.eof())))
		this->_response = line;
	ifs.close();

	this->_content_length = _response.size();
	this->_content_type = "text/html"; // a modifier avec une fonction en fonction du ype
	this->_date = set_date();

	set_header();
	//gerer le autoindex ?
}

void Response::run_post_method(void)
{
	//pour le moment fait la meme que get en attendant CGI
	// for (std::map<std::string, std::string>::iterator it = _body.begin(); it != _body.end(); it++)
	// 		std::cout << YELLOW << "map[" << it->first << "] = " << it->second << std::endl;
	run_get_method();
	for (std::map<std::string, std::string>::iterator it = _body.begin(); it != _body.end(); it++)
	{
		size_t pos = _response.find("</div>");
		std::string str = "\n<h4> " + it->first + "</h4>" + "<p>: " + it->second + "</p>";
		_response.insert(pos, str);
		this->_content_length = _response.size();
	}
	set_header();
	return ;
}

void	Response::set_header(void)
{
	this->_header = this->_http + " " + this->_code_status.second;
	this->_header += "\r\nContent-Length: " + ft_to_string(this->_content_length);
	this->_header += "\r\nContent-Location: " + this->_content_location;
	this->_header += "\r\nContent-Type: " + this->_content_type;
	this->_header += "\r\nDate: " + this->_date;
	this->_header += RED "\nServer: webserv\r\n\r\n" RESET;
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
