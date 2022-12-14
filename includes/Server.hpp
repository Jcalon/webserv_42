/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbascuna <mbascuna@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/08 10:01:10 by mbascuna          #+#    #+#             */
/*   Updated: 2022/12/08 11:07:21 by mbascuna         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
# define SERVER_HPP

#include "utils.hpp"
// #include "Location.hpp"

class Server
{
	public:
		Server(void);
		~Server(void);

		std::vector<std::string>::iterator 	parse_server(std::vector<std::string>::iterator start, std::vector<std::string> file);
		void 								init_error_pages(void);
		void 								init_allow_methods(void);

		std::string 						get_root(void) const;
		std::string 						get_ip(void) const;
		std::vector<std::string> 			get_listen(void) const;
		std::vector<std::string> 			get_name(void) const;
		std::map<int, std::string> 			get_error_pages(void) const;
		std::string 						get_body_size(void) const;
		std::string 						get_index(void) const;
		// std::string 						get_index_path(std::string location) const;
		std::string 						get_cgi_dir(void) const;
		bool 								get_autoindex(void) const;
		std::string						 	get_cgi_ext(void) const;
		std::vector<Location> 				get_location(void) const;
		std::vector<std::string> 			get_allow_method(void) const;
		bool								get_error(void) const;

		bool 								check_listen(std::vector<std::string> line);
		bool 								check_method(std::vector<std::string> line);
		bool 								check_error(std::vector<std::string> line);
		bool								check_auto(std::vector<std::string> line);

	private:
		std::string							_ip_address;
		std::vector<std::string>			_listen; 				// setup chaque server avec son port
		std::vector<std::string>			_name;
		std::string							_root;
		std::map<int, std::string>			_error_pages; 			// Change les pages d'erreur des codes pour le fichier path
		std::string							_max_client_body_size; 	// Taille maximale du body d'une requ??te
		std::string							_cgi_dir; 				//chemin du dossier contenant les executables CGI
		std::string							_cgi_ext;				// CGI binary that will be executed for the given extension.
		bool								_autoindex; 			// Si une requ??te est faite sur un dossier, l'autoindex liste les fichiers pr??sents dans le dossier
		std::string 						_index;					//defines files that will be used as an index.
		std::vector<std::string>			_allow_method;
		// bool								_auth_basic; 			// Change le nom de la popup d'authentification pour les ressources prot??g??es par un mot de passe
		// std::string						_auth_basic_user_file; 	// D??finit le fichier contenant les logins et mot de passes pour acc??der ?? la ressourc
		std::vector<Location>				_location; 				// Indique la cr??ation d'une location name, qui pourra ??tre utilis??e dans un url
		bool								_is_error;
};

std::ostream	&operator<<(std::ostream &o, Server const &Server);

#endif
