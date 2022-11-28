/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcalon <jcalon@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/21 17:55:06 by jcalon            #+#    #+#             */
/*   Updated: 2022/11/28 15:15:12 by jcalon           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/utils.hpp"

class throwMessage : public std::exception {
	private:
		std::string _msg;

	public:
		throwMessage (const std::string& msg) throw() : _msg(msg) {}
		throwMessage (const throwMessage&) throw() {}
		throwMessage& operator= (const throwMessage&) throw();
		virtual ~throwMessage() throw() {}
		virtual const char* what() const throw() { return (_msg.c_str()); }
};

void throwError(const std::exception& ex)
{
	std::cerr << "\033[1m\033[31mERROR \033[0m: " << ex.what() << '\n';
}

static bool isValidHTTPVersion(std::string http_version)
{
	if (http_version == "HTTP/1.1")
		return (true);
	return (false);
}

Request::Request(const std::string & request): _request(request), _fields(), _infos(), _body("")
{
	size_t start = 0;
	size_t end = request.find("\n");
	size_t pos;

	while (end != std::string::npos)
	{
		this->_fields.push_back(request.substr(start, end - start));
		start = end + 1;
		end = request.find("\n", start);
	}
	this->_fields.push_back(request.substr(start, end - start));
	try
	{
		if (this->_fields.size() < 1)
			throw(throwMessage("Not valid header (size to low)"));
		pos = this->_fields[0].find(" ");
		if (pos != std::string::npos)
		{
			this->_infos._method = this->_fields[0].substr(0, this->_fields[0].find(' '));
			if (this->_infos._method != "GET" && this->_infos._method != "POST" && this->_infos._method != "DELETE")
				throw(throwMessage("Request line not well formated (method)."));
			this->_fields[0].erase(0, this->_infos._method.length() + 1);
		}
		else
			throw(throwMessage("Not valid header "));

		pos = this->_fields[0].find(" ");
		if (pos != std::string::npos)
		{
			this->_infos._target = this->_fields[0].substr(0, this->_fields[0].find(' '));
			this->_fields[0].erase(0, this->_infos._target.length() + 1);
		}
		else
			throw(throwMessage("Not valid header "));

		pos = this->_fields[0].find("\r");
		if (pos != std::string::npos)
		{
			this->_infos._http = this->_fields[0].substr(0, this->_fields[0].find('\r'));
			if(isValidHTTPVersion(this->_infos._http) == false)
				throw(throwMessage("Request line not well formated (http version) : " + this->_infos._http));
		}
		else
			throw(throwMessage("Not valid header "));
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
	catch (const std::exception& e)
	{
		throwError(e);
		throw(throwMessage("Can't parse header."));
	}
}

Request::~Request()
{
}

std::string	Request::getRawRequest() const
{
	return this->_request;
}

std::vector<std::string>	Request::getFields() const
{
	return this->_fields;
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
