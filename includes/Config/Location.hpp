/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Location.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbascuna <mbascuna@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/21 11:37:28 by mbascuna          #+#    #+#             */
/*   Updated: 2022/11/21 12:18:40 by mbascuna         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LOCATION_HPP
# define LOCATION_HPP

#include <iostream>
#include <vector>
#include <map>

class Location
{
	public:
		Location(void);
		~Location(void);

	private:
		std::string							_root;
		std::vector<std::string>			_disable_method;
		int									_max_client_body_size;
		std::string 						_index;
		std::map<std::string, std::string>	_cgi_ext;
		bool								_autoindex;

};

#endif
