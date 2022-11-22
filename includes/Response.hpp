/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbascuna <mbascuna@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/22 17:48:22 by mbascuna          #+#    #+#             */
/*   Updated: 2022/11/22 18:17:54 by mbascuna         ###   ########.fr       */
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

		void build_response(Request const &request);

	private:
		std::map<std::string, std::string> _status_code;
		std::string _method;
		std::string _http;
		// RECUPERER LES FIELDS ?

};

#endif
