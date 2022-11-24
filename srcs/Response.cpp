/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbascuna <mbascuna@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/22 17:48:13 by mbascuna          #+#    #+#             */
/*   Updated: 2022/11/23 14:00:05 by mbascuna         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/utils.hpp"

Response::Response(Request const &request, Server const &server)
{
	//mettre dans map quand on aura fait la map des codes erreurs
	this->_code = allow_method(request);
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


int Response::allow_method(Request const &request)
{
	this->_method = request.getRequest()._method;
	//if _method n'est dans les method allow
		//envoyer code erreur correspondant
	return 200;
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
	return ;
}

void	Response::set_header(void)
{
	this->_header = this->_http + " " + ft_to_string(this->_code) + " OK";
	this->_header += "\nContent-Length: " + ft_to_string(this->_content_length);
	this->_header += "\nContent-Location: " + this->_content_location;
	this->_header += "\nContent-Type: " + this->_content_type;
	this->_header += "\nDate: " + this->_date;
	this->_header += RED "\nServer: ??????" RESET;
	this->_header += RED "\nTransfer-Encoding: ??????\n\r" RESET;
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
