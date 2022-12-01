/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbascuna <mbascuna@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/22 17:48:22 by mbascuna          #+#    #+#             */
/*   Updated: 2022/12/01 13:16:37 by mbascuna         ###   ########.fr       */
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

		std::pair<int, std::string>	allow_method(Request const &request, Server const &server, std::string loc_name);
		void 						call_method();
		void 						run_get_method(void);
		void 						run_post_method(void);
		void 						run_head_method(void);
		void 						run_put_method(void);
		void 						run_cgi_method(void);

		std::string 				get_header(void) const;
		std::string 				get_response(void) const;
		std::string					set_date(void);
		void						set_header(void);
		void 						parse_body(std::string fields);
		std::pair<int, std::string> find_pair(int code);
		bool						test_cgi(Server const &server, std::string loc_name);
		bool						is_cgi_in_extension(Server const &server);
		bool						is_cgi_in_location(Server const &server, std::string loc_name);
		bool						is_allowed_in_extension(Server const &server);
		bool						is_allowed_in_location(Server const &server, std::string loc_name);
		std::string					init_mime_types(void);
		void						load_error_pages(void);

		class FileNotOpen : public std::exception
		{
			public:
				virtual const char *what(void) const throw();
		};

	private:
		// std::map<std::string, std::string> 	_status_code;
		// int _code;
		Server 								_server;
		Request								_request;
		std::string 						_http;
		std::pair<int, std::string> 		_code_status;
		int									_content_length;
		std::string							_content_location;
		std::string							_content_type;
		std::string							_date;
		std::string 						_response;
		std::string							_header;
		std::string							_method;
		std::map<std::string, std::string> 	_body;
		bool								_cgi;
		std::string							_binary;
		std::string							_path;
		std::string 						_request_path;
};

#endif
