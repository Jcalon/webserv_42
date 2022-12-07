/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcalon <jcalon@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/21 17:55:06 by jcalon            #+#    #+#             */
/*   Updated: 2022/12/07 15:23:17 by jcalon           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/utils.hpp"

static bool isValidHTTPVersion(std::string http_version)
{
	if (http_version.find("HTTP/1.1") != std::string::npos)
		return (true);
	return (false);
}

Request::Request(const std::string & request): _request(request), _fields(), _infos(), _filename("") , _body("")
{
	size_t start = 0;
	size_t end = request.find("\n");
	size_t pos;

	_error = 0;
	while (end != std::string::npos)
	{
		this->_fields.push_back(request.substr(start, end - start));
		start = end + 1;
		end = request.find("\n", start);
	}
	this->_fields.push_back(request.substr(start, end - start));
	if (this->_fields.size() <= 3)
		_error = 400;
	pos = this->_fields[0].find(" ");
	if (pos != std::string::npos)
	{
		this->_infos._method = this->_fields[0].substr(0, this->_fields[0].find(' '));
		if (this->_infos._method != "GET" && this->_infos._method != "POST" && this->_infos._method != "HEAD" && this->_infos._method != "PUT" && this->_infos._method != "DELETE")
			_error = 405;
		this->_fields[0].erase(0, this->_infos._method.length() + 1);
	}
	else
		_error = 400;

	pos = this->_fields[0].find(" ");
	if (pos != std::string::npos)
	{
		std::string path = this->_fields[0].substr(0, this->_fields[0].find(' '));
		if (path.find('?') != std::string::npos)
		{
			this->_infos._target = path.substr(0, path.find('?'));
			this->_infos._query = path.substr(path.find('?'), path.find(' '));
			this->_fields[0].erase(0, this->_infos._query.length());
		}
		else
		{
			this->_infos._target = path;
			this->_infos._query = "";
		}
		this->_fields[0].erase(0, this->_infos._target.length() + 1);
	}
	else
		_error = 400;

	pos = this->_fields[0].find("\r");
	if (pos != std::string::npos)
	{
		this->_infos._http = this->_fields[0].substr(0, this->_fields[0].find('\r'));
		if(isValidHTTPVersion(this->_infos._http) == false)
			_error = 505;
		else if (this->_infos._http.length() > 8)
			_error = 400;
		this->_fields.erase(this->_fields.begin());
	}
	else
		_error = 400;
	if (_request.find("Transfer-Encoding: chunked") != std::string::npos)
		parseChunkedBody();
	else if (_request.find("boundary=") != std::string::npos)
	{
		this->_boundary = _request.substr(_request.find("boundary=") + 9, _request.find("\r\n" , _request.find("boundary=")) - _request.find("boundary=") - 9);
		parseBoundaryBody();
	}
	else
		parseBody();
}

Request::~Request()
{
}

void	Request::parseBody()
{
	std::vector<std::string>::iterator it = this->_fields.begin();
	for (; it != this->_fields.end(); it++)
	{
		if (*it == "\r")
		{
			it++;
			for (; it != this->_fields.end(); it++)
				this->_body += *it;
			break;
		}
	}
}

void	Request::parseChunkedBody()
{
	std::string	chunks = _request.substr(_request.find("\r\n\r\n") + 4, _request.size() - 1);
	std::string	subchunk = chunks.substr(0, 100);
	int			chunksize = strtol(subchunk.c_str(), NULL, 16);
	size_t		i = 0;
	while (chunksize)
	{
		i = chunks.find("\r\n", i) + 2;
		_body += chunks.substr(i, chunksize);
		i += chunksize + 2;
		subchunk = chunks.substr(i, 100);
		chunksize = strtol(subchunk.c_str(), NULL, 16);
	}
}

void	Request::parseBoundaryBody()
{
	std::vector<std::string> tmp = this->_fields;
	this->_fields.clear();
	int count = 0;
	for (std::vector<std::string>::iterator it = tmp.begin(); it != tmp.end(); it++)
	{
		if (it->find("filename=") != std::string::npos)
			this->_filename = it->substr(it->find("filename=") + 10, it->find("\"", it->find("filename=") + 10) - it->find("filename=") - 10);
		if (*it == "\r" && count == 0)
		{
			count++;
			it++;
		}
		else if (*it == "\r")
		{
			it++;
			for (; it != tmp.end(); it++)
			{
				if (it->find(_boundary) == std::string::npos)
				{
					this->_body += *it;
					this->_body += "\n";
				}
			}
			break;
		}
		else
			this->_fields.push_back(*it);
	}
}

std::string	Request::getRawRequest() const
{
	return this->_request;
}

int	Request::getError() const
{
	return this->_error;
}

std::vector<std::string>	Request::getFields() const
{
	return this->_fields;
}

std::string	Request::getFilename() const
{
	return this->_filename;
}

Request::request_info	Request::getRequest() const
{
	return this->_infos;
}

std::string	Request::getBody() const
{
	return this->_body;
}

std::ostream &operator<<(std::ostream &out, const Request &request)
{
	out << "START LINE = ";
	out << request.getRequest()._http << " | ";
	out << request.getRequest()._method;
	out << " | " << request.getRequest()._target << std::endl;
	if (request.getBody() != "")
		out << request.getBody() << std::endl;
	return (out);
}
