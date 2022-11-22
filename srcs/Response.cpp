/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbascuna <mbascuna@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/22 17:48:13 by mbascuna          #+#    #+#             */
/*   Updated: 2022/11/22 18:19:00 by mbascuna         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/utils.hpp"

Response::Response(Request const &request)
{
	build_response(request);
	//recupere le error code avec build response dans la map
}

Response::~Response() {}

void Response::build_response(Request const &request)
{
	// this->_method = request._method;
	//if method est allow
		// renvoi le code error


}
