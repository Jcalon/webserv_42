/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcalon <jcalon@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/21 11:36:00 by mbascuna          #+#    #+#             */
/*   Updated: 2022/11/23 11:47:09 by jcalon           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
# define SERVER_HPP

#include "Location.hpp"
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
		std::string 						get_index_path(std::string location) const;
		std::string 						get_cgi_dir(void) const;
		bool 								get_autoindex(void) const;
		std::map<std::string, std::string> 	get_cgi_ext(void) const;
		std::vector<Location> 				get_location(void) const;
		std::vector<std::string> 			get_allow_method(void) const;

	private:
		std::string							_ip_address;
		std::vector<std::string>			_listen; 				// setup chaque server avec son port
		std::vector<std::string>			_name;
		std::string							_root;
		std::map<int, std::string>			_error_pages; 			// Change les pages d'erreur des codes pour le fichier path
		std::string							_max_client_body_size; 	// Taille maximale du body d'une requête
		std::string							_cgi_dir; 				//chemin du dossier contenant les executables CGI
		std::map<std::string, std::string>	_cgi_ext;				// CGI binary that will be executed for the given extension.
		bool								_autoindex; 			// Si une requête est faite sur un dossier, l'autoindex liste les fichiers présents dans le dossier
		std::string 						_index;					//defines files that will be used as an index.
		std::vector<std::string>			_allow_method;
		// bool								_auth_basic; 			// Change le nom de la popup d'authentification pour les ressources protégées par un mot de passe
		// std::string						_auth_basic_user_file; 	// Définit le fichier contenant les logins et mot de passes pour accéder à la ressourc
		std::vector<Location>				_location; 				// Indique la création d'une location name, qui pourra être utilisée dans un url
};

std::ostream	&operator<<(std::ostream &o, Server const &Server);

#endif
