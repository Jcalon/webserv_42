/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcalon <jcalon@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/22 17:48:22 by mbascuna          #+#    #+#             */
/*   Updated: 2022/11/30 13:34:31 by jcalon           ###   ########.fr       */
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

		std::string 				get_header(void) const;
		std::string 				get_response(void) const;
		std::string					set_date(void);
		void						set_header(void);
		void 						parse_body(std::string fields);
		std::pair<int, std::string> find_pair(int code);

		bool						is_allowed_in_extension(Server const &server);
		bool						is_allowed_in_location(Server const &server, std::string loc_name);
		std::string					init_mime_types(void);

		class FileNotOpen : public std::exception
		{
			public:
				virtual const char *what(void) const throw();
		};

	private:
		// std::map<std::string, std::string> 	_status_code;
		// int _code;
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
};

#endif
