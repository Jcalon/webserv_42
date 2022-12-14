/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Autoindex.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbascuna <mbascuna@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/01 11:55:43 by mbascuna          #+#    #+#             */
/*   Updated: 2022/12/07 16:52:12 by mbascuna         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef AUTOINDEX_HPP
# define AUTOINDEX_HPP

#include "utils.hpp"

class Autoindex
{
	public:
		Autoindex(Server const &serv, std::string const &path);
		~Autoindex(void);

		std::string			get_html(void) const;
		void 				read_directory(void);
		void				creation_href(void);
		void				insert_html(void);

	private:
		Server						_serv;
		std::vector<std::string>	_list_name;
		std::vector<std::string>	_list_href;
		std::string					_html;
		std::string					_path;

};

#endif
