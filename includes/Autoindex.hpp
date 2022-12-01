/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Autoindex.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbascuna <mbascuna@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/01 11:55:43 by mbascuna          #+#    #+#             */
/*   Updated: 2022/12/01 12:27:26 by mbascuna         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef AUTOINDEX_HPP
# define AUTOINDEX_HPP

#include "utils.hpp"

class Autoindex
{
	public:
		Autoindex(void);
		Autoindex(char *argv);
		~Autoindex(void);

	private:
		std::vector<std::string>	_list;
		std::string					_html;
		std::string					_path;

};

std::ostream &operator<<(std::ostream &o, Config const &rhs);

#endif
