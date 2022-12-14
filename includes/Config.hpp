/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Config.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbascuna <mbascuna@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/21 11:34:05 by mbascuna          #+#    #+#             */
/*   Updated: 2022/12/07 14:28:42 by mbascuna         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CONFIG_HPP
# define CONFIG_HPP

#include "utils.hpp"

class Config
{
	public:
		Config(void);
		Config(char *argv);
		~Config(void);

		void				parse_config(std::vector<std::string> file);
		std::string 		get_workers(void) const;
		std::vector<Server>	get_server(void) const;
		std::string 		get_max_connections(void) const;
		bool				get_error(void) const;

		class FileNotOpen : public std::exception
		{
			public:
				virtual const char *what(void) const throw();
		};

		std::vector<Server>::iterator check_server(std::vector<std::string> listen);
	private:
		std::vector<Server>	_server;
		std::string 	_workers;
		std::string		_max_connections;
		bool			_is_error;

};

std::ostream &operator<<(std::ostream &o, Config const &rhs);

#endif
