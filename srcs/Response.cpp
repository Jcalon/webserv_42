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

Response::Response(Request const &request)
{
	//mettre dans map quand on aura fait la map des codes erreurs
	this->_code = allow_method(request);
	this->_http = request.getRequest()._http;
	this->_response = "";
	this->_content_length = "";
	this->_content_location = request.getRequest()._target;
	this->_content_type = "";
	this->_header = "";
}

Response::~Response() {
}

int Response::allow_method(Request const &request)
{
	this->_method = request.getRequest()._method;
	//if _method n'est dans les method allow
		//envoyer code erreur correspondant
	return 200;
}

std::string Response::get_header(void) const
{
	return this->_header;
}

std::string Response::get_response(void) const
{
	return this->_response;
}


void Response::call_method()
{
	if (this->_method == "GET")
		run_get_method();
	// else if (method == "POST")
	// 	run_post_method();
	// else if (method == "DELETE")
	// 	run_delete_method();
}

void Response::run_get_method(void)
{
	// if (this->_content_location)

	//si le path est un fichier dans notre code
		//open le fichier est mettre le contenu dans la string_response
	std::ifstream		ifs("test.html");
	// std::string	buffer;
	std::string	line;
	if (!ifs.is_open())
	{
		std::cout << "ERROR opening html" << std::endl;
		return ;
	}
	while (std::getline(ifs, line, char(ifs.eof())))
	{
		this->_response = line;
	}
	// buffer << ifs.rdbuf();
	// this->_response = "<!DOCTYPE html><html><title>TEST</title><body><div><H1>TEST</H1><p>Coucou le test</p></div></body></html>;";
	// this->_response = buffer;
	ifs.close();
	this->_content_length = _response.size();
	this->_content_type = "text/html"; // a modifier avec une fonction en fonction du ype
	this->_date = set_date();

	// _header = this->_http + " " + this->_code.str() + " " + "\r";
	this->_header = this->_http + " " + "200" + " " + "\r\n";
	// header += writeHeader();
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
