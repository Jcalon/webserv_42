/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcalon <jcalon@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/21 15:52:28 by jcalon            #+#    #+#             */
/*   Updated: 2022/12/06 22:28:04 by jcalon           ###   ########.fr       */
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
		std::string					_boundary;
		std::vector<std::string>	_fields;
		struct request_info			_infos;
		std::string					_filename;
		std::string					_body;
		int							_error;
		void						parseBody();
		void						parseChunkedBody();
		void						parseBoundaryBody();

	public:
		Request(const std::string &request);
		~Request();
		std::string					getRawRequest() const;
		std::vector<std::string> 	getFields() const;
		struct request_info			getRequest() const;
		std::string					getBody() const;
		std::string					getFilename() const;
		int							getError() const;

};

std::ostream &operator<<(std::ostream &out, const Request &request);

#endif
