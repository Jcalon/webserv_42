/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Config.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbascuna <mbascuna@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/21 11:34:05 by mbascuna          #+#    #+#             */
/*   Updated: 2022/11/21 17:35:20 by mbascuna         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CONFIG_HPP
# define CONFIG_HPP

#include <iostream>
#include <vector>
#include <iterator>
#include <string>
#include "Server.hpp"

class Config
{
	public:
		Config(void);
		Config(std::vector<std::string> file);
		~Config(void);

		void				parse_config(std::vector<std::string> file);
		std::string 		get_workers(void) const;
		std::vector<Server>	get_server(void) const;
		std::string 		get_max_connections(void) const;

	private:
		std::vector<Server>	_server;
		std::string 	_workers;
		std::string		_max_connections;

};

std::vector<std::string> ft_cpp_split(const std::string &str, const std::string &charset);
bool	ft_in_charset(char const c, const std::string &charset);

std::ostream &operator<<(std::ostream &o, Config const &rhs);

#endif
