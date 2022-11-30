/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Location.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcalon <jcalon@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/21 11:37:28 by mbascuna          #+#    #+#             */
/*   Updated: 2022/11/30 17:21:24 by jcalon           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LOCATION_HPP
# define LOCATION_HPP

#include "utils.hpp"

class Server;

class Location
{
	public:
		Location(Server &server);
		~Location(void);

		std::vector<std::string>::iterator 	parse_location(std::vector<std::string>::iterator start, std::vector<std::string> file);
		void 								init_allow_methods(void);

		std::string 						get_name(void) const;
		std::string 						get_root(void) const;
		std::string 						get_upload(void) const;
		std::vector<std::string> 			get_allow_method(void) const;
		std::string 						get_body_size(void) const;
		std::string 						get_index(void) const;
		std::map<std::string, std::string>	get_cgi_ext(void) const;
		std::string							get_cgi_dir(void) const;
		bool 								get_autoindex(void) const;

	private:
		std::string							_name;
		std::string							_root;
		std::string							_upload;
		bool								_autoindex;
		std::string 						_index;
		std::map<std::string, std::string>	_cgi_ext;
		std::string							_cgi_dir;
		std::vector<std::string>			_allow_method;
		std::string							_max_client_body_size;
};

std::ostream	&operator<<(std::ostream &o, Location const &server);

#endif
