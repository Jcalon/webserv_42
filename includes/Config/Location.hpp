/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Location.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbascuna <mbascuna@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/21 11:37:28 by mbascuna          #+#    #+#             */
/*   Updated: 2022/11/21 17:48:14 by mbascuna         ###   ########.fr       */
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

		std::vector<std::string>::iterator parse_location(std::vector<std::string>::iterator start, std::vector<std::string> file);

		std::string get_root(void) const;
		std::string get_upload(void) const;
		std::vector<std::string> get_disable_method(void) const;
		std::string get_body_size(void) const;
		std::string get_index(void) const;
		std::map<std::string, std::string> get_cgi_ext(void) const;
		bool get_autoindex(void) const;

	private:
		std::string							_root;
		std::string							_upload;
		std::vector<std::string>			_disable_method;
		std::string							_max_client_body_size;
		std::string 						_index;
		std::map<std::string, std::string>	_cgi_ext;
		bool								_autoindex;

};

std::ostream	&operator<<(std::ostream &o, Location const &server);

#endif
