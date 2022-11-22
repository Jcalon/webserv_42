/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbascuna <mbascuna@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/21 15:52:28 by jcalon            #+#    #+#             */
/*   Updated: 2022/11/22 13:43:02 by mbascuna         ###   ########.fr       */
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
			std::string _http;
		};

	private:
		std::string					_request;
		std::vector<std::string>	_fields;
		struct request_info			_infos;

	public:
		Request(const std::string &request);
		~Request();
		std::string					getRawRequest() const;
		std::vector<std::string> 	getFields() const;
		struct request_info			getRequest() const;

};

std::ostream &operator<<(std::ostream &out, const Request &request);

#endif
