/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbascuna <mbascuna@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/22 17:48:22 by mbascuna          #+#    #+#             */
/*   Updated: 2022/11/23 13:22:55 by mbascuna         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef RESPONSE_HPP
# define RESPONSE_HPP

#include "utils.hpp"

class Response
{
	public:
		Response(Request const &request, Server const &server);
		~Response();

		std::pair<int, std::string> allow_method(Request const &request);
		void call_method();
		void run_get_method(void);
		void run_post_method(void);

		std::string get_header(void) const;
		std::string get_response(void) const;
		std::string		set_date(void);
		void			set_header(void);
		void 	parse_body(std::vector<std::string> fields);
		std::pair<int, std::string> find_pair(int code);


		class FileNotOpen : public std::exception
		{
			public:
				virtual const char *what(void) const throw();
		};

	private:
		std::map<std::string, std::string> _status_code;
		std::string _http;
		// int _code;
		std::pair<int, std::string> _code_status;
		int	_content_length;
		std::string	_content_location;
		std::string	_content_type;
		std::string	_date;
		std::string _response;
		std::string _header;
		std::string _method;
		std::map<std::string, std::string> _body;
};

#endif
