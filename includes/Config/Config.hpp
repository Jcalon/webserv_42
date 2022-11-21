/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Config.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbascuna <mbascuna@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/21 11:34:05 by mbascuna          #+#    #+#             */
/*   Updated: 2022/11/21 11:45:11 by mbascuna         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CONFIG_HPP
# define CONFIG_HPP

#include <iostream>
#include <vector>
#include "Server.hpp"

class Config
{
	public:
		Config(void);
		~Config(void);

	private:
		std::vector<Server>	_server;
		int 	_workers;
		int		_max_connections;

};

#endif
