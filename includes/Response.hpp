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
		Response(Request const &request);
		~Response();

		int allow_method(Request const &request);
		void call_method();
		void run_get_method(void);

		std::string get_header(void) const;
		std::string get_response(void) const;
		std::string		set_date(void);

	private:
		std::map<std::string, std::string> _status_code;
		std::string _http;
		int _code;
		std::string	_content_length;
		std::string	_content_location;
		std::string	_content_type;
		std::string	_date;
		std::string _response;
		std::string _header;
		std::string _method;
};

#endif
