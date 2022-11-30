/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcalon <jcalon@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/21 15:52:28 by jcalon            #+#    #+#             */
/*   Updated: 2022/11/30 18:16:58 by jcalon           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef REQUEST_HPP
# define REQUEST_HPP

#include "utils.hpp"

class Request
{
	public:
		struct request_info
		{
			std::string _method;
			std::string _target;
			std::string _query;
			std::string _http;
		};

	private:
		std::string					_request;
		std::vector<std::string>	_fields;
		struct request_info			_infos;
		std::string					_body;
		void						parseBody();
		void						parseChunkedBody();

	public:
		Request(const std::string &request);
		~Request();
		std::string					getRawRequest() const;
		std::vector<std::string> 	getFields() const;
		struct request_info			getRequest() const;
		std::string					getBody() const;

};

std::ostream &operator<<(std::ostream &out, const Request &request);

#endif
