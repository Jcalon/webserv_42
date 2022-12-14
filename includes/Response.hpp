/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcalon <jcalon@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/01 16:05:37 by mbascuna          #+#    #+#             */
/*   Updated: 2022/12/07 20:16:39 by jcalon           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef RESPONSE_HPP
# define RESPONSE_HPP

#include "utils.hpp"

class Response
{
	public:
		Response(Request const &request, int error);
		Response(Request const &request, Server const &server);
		~Response();

		std::pair<int, std::string>	allow_method(Request const &request, Server const &server, std::string loc_name);
		void 						call_method();
		void 						run_get_method(void);
		void 						run_post_method(void);
		void 						run_head_method(void);
		void 						run_put_method(void);
		void 						run_cgi_method(void);
		void						run_delete_method(void);

		std::string 				get_header(void) const;
		std::string 				get_response(void) const;
		std::string					set_date(void);
		void						set_header(void);
		void 						parse_body(std::string fields);
		std::pair<int, std::string> find_pair(int code);
		bool						test_cgi(Server const &server, std::string loc_name);
		bool						is_cgi_in_extension(Server const &server);
		bool						is_cgi_in_location(Server const &server, std::string loc_name, std::string ext);
		bool						is_allowed_in_extension(Server const &server);
		bool						is_allowed_in_location(Server const &server, std::string loc_name);
		bool						is_max_size_in_extension(Request const &request, Server const &server);
		bool						is_max_size_in_location(Request const &request, Server const &server, std::string loc_name);
		std::string					init_mime_types(void);
		void						load_error_pages(void);
		std::string 				get_index_path(std::string location) const;
		// void 						construct_header(void);

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
